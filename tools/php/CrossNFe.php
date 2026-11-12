<?php

declare(strict_types=1);

namespace CrossNFe;

use FFI;
use FFI\Exception as FfiException;
use InvalidArgumentException;
use RuntimeException;

final class Session
{
    private const ABI = 1;
    private const OK = 0;
    private const BUFFER = 4;
    private const API = <<<'C'
        typedef uint64_t cn_handle;
        uint32_t cn_abi_version(void);
        int cn_create(cn_handle *root);
        int cn_destroy(cn_handle root);
        int cn_child(cn_handle parent, const char *name, cn_handle *child);
        int cn_add(cn_handle collection, cn_handle *item);
        int cn_count(cn_handle collection, int32_t *count);
        int cn_at(cn_handle collection, int32_t index, cn_handle *item);
        int cn_get(cn_handle object, const char *property, char *output, size_t capacity, size_t *required);
        int cn_set(cn_handle object, const char *property, const char *value);
        int cn_execute(cn_handle object, const char *operation, const char *argument);
        int cn_error(char *output, size_t capacity, size_t *required);
        C;

    private FFI $ffi;
    private ?int $handle = null;

    public function __construct()
    {
        if (PHP_INT_SIZE !== 8 || !in_array(PHP_OS_FAMILY, ['Windows', 'Linux'], true)) {
            throw new RuntimeException('O exemplo requer PHP de 64 bits no Windows ou Linux x64.');
        }
        if (!extension_loaded('ffi')) {
            throw new RuntimeException('Habilite a extensão FFI no PHP CLI.');
        }
        $nome = PHP_OS_FAMILY === 'Windows' ? 'crossnfe.dll' : 'libcrossnfe.so';
        $arquivo = getenv('CROSSNFE_LIBRARY') ?: dirname(__DIR__) . DIRECTORY_SEPARATOR . $nome;
        $biblioteca = realpath($arquivo);
        if ($biblioteca === false || !is_file($biblioteca)) {
            throw new RuntimeException("Biblioteca CrossNFe não encontrada: $arquivo");
        }
        try {
            $this->ffi = FFI::cdef(self::API, $biblioteca);
        } catch (FfiException $erro) {
            throw new RuntimeException(
                "Falha ao carregar $biblioteca. Confira ffi.enable e as dependências nativas. " . $erro->getMessage(),
                0,
                $erro
            );
        }
        $versao = $this->ffi->cn_abi_version();
        if ($versao !== self::ABI) {
            throw new RuntimeException("ABI CrossNFe $versao incompatível com este exemplo.");
        }
        $raiz = $this->ffi->new('cn_handle');
        $this->conferir($this->ffi->cn_create(FFI::addr($raiz)));
        $this->handle = $raiz->cdata;
    }

    public function root(): Node
    {
        $this->api();
        return new Node($this, $this->handle);
    }

    public function close(): void
    {
        if ($this->handle !== null) {
            $this->conferir($this->ffi->cn_destroy($this->handle));
            $this->handle = null;
        }
    }

    public function __destruct()
    {
        if ($this->handle !== null) {
            $this->ffi->cn_destroy($this->handle);
        }
    }

    /** @internal Usado pelos objetos da sessão. */
    public function api(): FFI
    {
        if ($this->handle === null) {
            throw new RuntimeException('Sessão CrossNFe encerrada.');
        }
        return $this->ffi;
    }

    /** @internal Lê a mensagem antes que outra chamada substitua o erro nativo. */
    public function conferir(int $status, string $contexto = ''): void
    {
        if ($status === self::OK) {
            return;
        }
        $mensagem = $this->lerTexto(
            fn ($buffer, $capacidade, $tamanho) => $this->ffi->cn_error($buffer, $capacidade, $tamanho),
            mensagemDeErro: true
        );
        $prefixo = $contexto === '' ? '' : "$contexto: ";
        throw new RuntimeException("CrossNFe ($status): $prefixo$mensagem", $status);
    }

    /** @internal A ABI informa o tamanho do buffer incluindo o NUL final. */
    public function lerTexto(callable $ler, string $contexto = '', bool $mensagemDeErro = false): string
    {
        $tamanho = $this->ffi->new('size_t');
        $ponteiro = FFI::addr($tamanho);
        $status = $ler(null, 0, $ponteiro);
        if ($status !== self::BUFFER) {
            if ($mensagemDeErro) {
                return 'Mensagem nativa indisponível.';
            }
            $this->conferir($status, $contexto);
        }
        $capacidade = $tamanho->cdata;
        if ($capacidade < 1) {
            throw new RuntimeException('Tamanho de texto inválido devolvido pela CrossNFe.');
        }
        $buffer = $this->ffi->new("char[$capacidade]");
        $status = $ler($buffer, $capacidade, $ponteiro);
        if ($mensagemDeErro && $status !== self::OK) {
            return 'Mensagem nativa indisponível.';
        }
        $this->conferir($status, $contexto);
        return FFI::string($buffer, $tamanho->cdata - 1);
    }
}

/** Referência emprestada da sessão para um objeto ou uma coleção da CppNFe. */
final class Node
{
    public function __construct(private Session $session, private int $handle)
    {
    }

    public function child(string $nome): self
    {
        $ffi = $this->session->api();
        $saida = $ffi->new('cn_handle');
        $this->session->conferir(
            $ffi->cn_child($this->handle, self::texto($nome), FFI::addr($saida)),
            "acessar grupo $nome"
        );
        return new self($this->session, $saida->cdata);
    }

    public function add(): self
    {
        $ffi = $this->session->api();
        $saida = $ffi->new('cn_handle');
        $this->session->conferir($ffi->cn_add($this->handle, FFI::addr($saida)));
        return new self($this->session, $saida->cdata);
    }

    public function at(int $indice): self
    {
        if ($indice < 0 || $indice > 2147483647) {
            throw new InvalidArgumentException('Índice fora da faixa da ABI.');
        }
        $ffi = $this->session->api();
        $saida = $ffi->new('cn_handle');
        $this->session->conferir($ffi->cn_at($this->handle, $indice, FFI::addr($saida)));
        return new self($this->session, $saida->cdata);
    }

    public function count(): int
    {
        $ffi = $this->session->api();
        $quantidade = $ffi->new('int32_t');
        $this->session->conferir($ffi->cn_count($this->handle, FFI::addr($quantidade)));
        return $quantidade->cdata;
    }

    public function get(string $campo): string
    {
        $ffi = $this->session->api();
        $campo = self::texto($campo);
        return $this->session->lerTexto(
            fn ($buffer, $capacidade, $tamanho) => $ffi->cn_get(
                $this->handle, $campo, $buffer, $capacidade, $tamanho
            ),
            "ler $campo"
        );
    }

    public function set(string $campo, string|int|float|bool $valor): void
    {
        $texto = match (true) {
            is_float($valor) => json_encode($valor, JSON_THROW_ON_ERROR),
            is_bool($valor) => $valor ? '1' : '0',
            default => (string) $valor,
        };
        $ffi = $this->session->api();
        $this->session->conferir(
            $ffi->cn_set($this->handle, self::texto($campo), self::texto($texto)),
            "preencher $campo"
        );
    }

    /** @param array<string, string|int|float|bool> $campos */
    public function fill(array $campos): void
    {
        foreach ($campos as $campo => $valor) {
            $this->set($campo, $valor);
        }
    }

    public function execute(string $operacao, string $argumento = ''): void
    {
        $ffi = $this->session->api();
        $this->session->conferir(
            $ffi->cn_execute($this->handle, self::texto($operacao), self::texto($argumento)),
            "executar $operacao"
        );
    }

    private static function texto(string $valor): string
    {
        if (str_contains($valor, "\0")) {
            throw new InvalidArgumentException('O texto contém um byte NUL.');
        }
        return $valor;
    }
}
