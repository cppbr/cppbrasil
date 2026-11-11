"""Acesso à biblioteca nativa da CrossNFe por ctypes, sem dependências externas.

A biblioteca é procurada, nesta ordem:

1. no caminho da variável de ambiente ``CROSSNFE_LIBRARY`` (arquivo da biblioteca);
2. na pasta deste pacote e nas duas pastas acima dela;
3. pelo carregador do sistema (``LD_LIBRARY_PATH``, ``PATH``, ``ldconfig``).
"""
from __future__ import annotations

import ctypes
import ctypes.util
import os
import sys
import threading
from pathlib import Path
from typing import TYPE_CHECKING, Callable, Generic, Iterator, Optional, TypeVar, overload

__all__ = ["CrossNFeError", "Session", "Node", "Model", "Items", "library_path"]

CN_OK, CN_ERROR, CN_INVALID_HANDLE, CN_ARGUMENT, CN_BUFFER, CN_THREAD = range(6)
ABI_VERSION = 1


class CrossNFeError(RuntimeError):
    """Erro devolvido pela CrossNFe. ``status`` traz o código ``cn_status``."""

    def __init__(self, status: int, message: str) -> None:
        super().__init__(f"CrossNFe ({status}): {message}")
        self.status = status


def _library_name() -> str:
    if sys.platform == "win32":
        return "crossnfe.dll"
    if sys.platform == "darwin":
        return "libcrossnfe.dylib"
    return "libcrossnfe.so"


def library_path() -> Optional[str]:
    """Caminho da biblioteca nativa que será carregada, ou ``None`` para o carregador do sistema."""
    env = os.environ.get("CROSSNFE_LIBRARY")
    if env:
        return str(Path(env).resolve())
    here = Path(__file__).resolve().parent
    for folder in (here, here.parent, here.parent.parent):
        candidate = folder / _library_name()
        if candidate.is_file():
            return str(candidate)
    return None


def _load() -> ctypes.CDLL:
    path = library_path()
    if path is None:
        path = ctypes.util.find_library("crossnfe") or _library_name()
    if sys.platform == "win32":
        # Procura as dependências (CppNFe.dll, Qt, libxml2, OpenSSL) na pasta da DLL e no PATH.
        lib = ctypes.CDLL(path, winmode=0x00000008)  # LOAD_WITH_ALTERED_SEARCH_PATH
    else:
        lib = ctypes.CDLL(path)
    handle_p = ctypes.POINTER(ctypes.c_uint64)
    size_p = ctypes.POINTER(ctypes.c_size_t)
    signatures = {
        "cn_abi_version": ([], ctypes.c_uint32),
        "cn_create": ([handle_p], ctypes.c_int),
        "cn_destroy": ([ctypes.c_uint64], ctypes.c_int),
        "cn_child": ([ctypes.c_uint64, ctypes.c_char_p, handle_p], ctypes.c_int),
        "cn_add": ([ctypes.c_uint64, handle_p], ctypes.c_int),
        "cn_count": ([ctypes.c_uint64, ctypes.POINTER(ctypes.c_int32)], ctypes.c_int),
        "cn_at": ([ctypes.c_uint64, ctypes.c_int32, handle_p], ctypes.c_int),
        "cn_clear": ([ctypes.c_uint64], ctypes.c_int),
        "cn_get": ([ctypes.c_uint64, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_size_t, size_p], ctypes.c_int),
        "cn_set": ([ctypes.c_uint64, ctypes.c_char_p, ctypes.c_char_p], ctypes.c_int),
        "cn_execute": ([ctypes.c_uint64, ctypes.c_char_p, ctypes.c_char_p], ctypes.c_int),
        "cn_error": ([ctypes.c_char_p, ctypes.c_size_t, size_p], ctypes.c_int),
    }
    for name, (args, result) in signatures.items():
        fn = getattr(lib, name)
        fn.argtypes = args
        fn.restype = result
    version = lib.cn_abi_version()
    if version != ABI_VERSION:
        raise CrossNFeError(CN_ERROR, f"ABI {version} incompatível; este binding usa a ABI {ABI_VERSION}")
    return lib


_lib: Optional[ctypes.CDLL] = None
_lib_lock = threading.Lock()


def _native() -> ctypes.CDLL:
    global _lib
    if _lib is None:
        with _lib_lock:
            if _lib is None:
                _lib = _load()
    return _lib


def _text(value: str) -> bytes:
    if not isinstance(value, str):
        raise TypeError("esperado str")
    if "\0" in value:
        raise ValueError("NUL não é suportado")
    return value.encode("utf-8")


def _check(status: int) -> None:
    if status == CN_OK:
        return
    lib = _native()
    size = ctypes.c_size_t()
    lib.cn_error(None, 0, ctypes.byref(size))
    buffer = ctypes.create_string_buffer(max(size.value, 1))
    lib.cn_error(buffer, len(buffer), ctypes.byref(size))
    raise CrossNFeError(status, buffer.value.decode("utf-8", "replace"))


class Session:
    """Instância da CppNFe. Use com ``with`` e sempre na thread que a criou.

    Crie a primeira sessão na thread principal: ela inicializa o ``QCoreApplication``.
    Os objetos obtidos a partir de ``root`` deixam de valer depois de ``close()``.
    """

    def __init__(self) -> None:
        self._thread = threading.get_ident()
        handle = ctypes.c_uint64()
        _check(_native().cn_create(ctypes.byref(handle)))
        self._handle = handle.value

    @property
    def root(self) -> "CppNFe":
        from .models import CppNFe

        self._ensure()
        return CppNFe(self, self._handle)

    @property
    def closed(self) -> bool:
        return self._handle == 0

    def close(self) -> None:
        if self._handle == 0:
            return
        self._ensure()
        _check(_native().cn_destroy(self._handle))
        self._handle = 0

    def __enter__(self) -> "Session":
        return self

    def __exit__(self, *exc: object) -> None:
        self.close()

    def _ensure(self) -> None:
        if self._handle == 0:
            raise CrossNFeError(CN_INVALID_HANDLE, "Sessão encerrada")
        if threading.get_ident() != self._thread:
            raise CrossNFeError(CN_THREAD, "Use a sessão na thread que a criou")


class Node:
    """Objeto ou coleção da árvore da CppNFe."""

    __slots__ = ("_session", "_handle")

    def __init__(self, session: Session, handle: int) -> None:
        self._session = session
        self._handle = handle

    @property
    def handle(self) -> int:
        self._session._ensure()
        return self._handle

    def _child(self, name: str) -> int:
        out = ctypes.c_uint64()
        _check(_native().cn_child(self.handle, _text(name), ctypes.byref(out)))
        return out.value

    def _get(self, name: str) -> str:
        lib = _native()
        key = _text(name)
        size = ctypes.c_size_t()
        status = lib.cn_get(self.handle, key, None, 0, ctypes.byref(size))
        if status != CN_BUFFER:
            _check(status)
        buffer = ctypes.create_string_buffer(size.value)
        _check(lib.cn_get(self.handle, key, buffer, size.value, ctypes.byref(size)))
        return buffer.value.decode("utf-8")

    def _set(self, name: str, value: str) -> None:
        _check(_native().cn_set(self.handle, _text(name), _text(value)))

    def clear(self) -> None:
        """Limpa o objeto. Invalida as referências obtidas para os descendentes."""
        _check(_native().cn_clear(self.handle))

    def execute(self, operation: str, argument: str = "") -> None:
        """Executa uma operação (``gerar``, ``assinar``, ``validar``, ``importar``, ``carregar``...)."""
        _check(_native().cn_execute(self.handle, _text(operation), _text(argument)))

    def __eq__(self, other: object) -> bool:
        return isinstance(other, Node) and other._session is self._session and other._handle == self._handle

    def __hash__(self) -> int:
        return hash((id(self._session), self._handle))

    def __repr__(self) -> str:
        return f"<{type(self).__name__} handle={self._handle}>"


class Model(Node):
    __slots__ = ()


M = TypeVar("M", bound=Model)


class Items(Node, Generic[M]):
    """Coleção da CppNFe (``Container<T>``). Índices começam em zero."""

    __slots__ = ("_factory",)

    def __init__(self, session: Session, handle: int, factory: Callable[[Session, int], M]) -> None:
        super().__init__(session, handle)
        self._factory = factory

    def add(self) -> M:
        """Cria um item já inserido na coleção e o devolve."""
        out = ctypes.c_uint64()
        _check(_native().cn_add(self.handle, ctypes.byref(out)))
        return self._factory(self._session, out.value)

    def __len__(self) -> int:
        out = ctypes.c_int32()
        _check(_native().cn_count(self.handle, ctypes.byref(out)))
        return out.value

    @overload
    def __getitem__(self, index: int) -> M: ...
    @overload
    def __getitem__(self, index: slice) -> list[M]: ...

    def __getitem__(self, index):  # type: ignore[no-untyped-def]
        if isinstance(index, slice):
            return [self[i] for i in range(*index.indices(len(self)))]
        if index < 0:
            index += len(self)
        out = ctypes.c_uint64()
        _check(_native().cn_at(self.handle, index, ctypes.byref(out)))
        return self._factory(self._session, out.value)

    def __iter__(self) -> Iterator[M]:
        for i in range(len(self)):
            yield self[i]


T = TypeVar("T")


class _Scalar(Generic[T]):
    """Propriedade escalar: converte entre o texto da ABI e o tipo Python."""

    __slots__ = ("name", "parse", "format", "write")

    def __init__(self, name: str, parse: Callable[[str], T], format: Callable[[T], str], write: bool) -> None:
        self.name, self.parse, self.format, self.write = name, parse, format, write

    @overload
    def __get__(self, obj: None, owner: type) -> "_Scalar[T]": ...
    @overload
    def __get__(self, obj: Node, owner: type) -> T: ...

    def __get__(self, obj, owner):  # type: ignore[no-untyped-def]
        if obj is None:
            return self
        return self.parse(obj._get(self.name))

    def __set__(self, obj: Node, value: T) -> None:
        if not self.write:
            raise AttributeError(f"{self.name} é somente leitura")
        obj._set(self.name, self.format(value))


def _format_int(value: int) -> str:
    if isinstance(value, bool) or not isinstance(value, int):
        value = int(value)
    return str(value)


def _format_float(value: float) -> str:
    return repr(float(value))


def _format_bool(value: bool) -> str:
    return "1" if value else "0"


def _format_str(value: str) -> str:
    if not isinstance(value, str):
        raise TypeError("esperado str")
    return value


def _str(name: str, write: bool = True) -> _Scalar[str]:
    return _Scalar(name, str, _format_str, write)


def _int(name: str, write: bool = True) -> _Scalar[int]:
    return _Scalar(name, int, _format_int, write)


def _float(name: str, write: bool = True) -> _Scalar[float]:
    return _Scalar(name, float, _format_float, write)


def _bool(name: str, write: bool = True) -> _Scalar[bool]:
    return _Scalar(name, lambda s: s == "1", _format_bool, write)


class _Child(Generic[M]):
    """Objeto individual (``std::unique_ptr<T>``)."""

    __slots__ = ("name", "type_name")

    def __init__(self, name: str, type_name: str) -> None:
        self.name, self.type_name = name, type_name

    @overload
    def __get__(self, obj: None, owner: type) -> "_Child[M]": ...
    @overload
    def __get__(self, obj: Node, owner: type) -> M: ...

    def __get__(self, obj, owner):  # type: ignore[no-untyped-def]
        if obj is None:
            return self
        from . import models

        return getattr(models, self.type_name)(obj._session, obj._child(self.name))


class _Collection(Generic[M]):
    """Coleção (``std::unique_ptr<Container<T>>``)."""

    __slots__ = ("name", "type_name")

    def __init__(self, name: str, type_name: str) -> None:
        self.name, self.type_name = name, type_name

    @overload
    def __get__(self, obj: None, owner: type) -> "_Collection[M]": ...
    @overload
    def __get__(self, obj: Node, owner: type) -> Items[M]: ...

    def __get__(self, obj, owner):  # type: ignore[no-untyped-def]
        if obj is None:
            return self
        from . import models

        return Items(obj._session, obj._child(self.name), getattr(models, self.type_name))


if TYPE_CHECKING:
    from .models import CppNFe
