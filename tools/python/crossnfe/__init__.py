"""CrossNFe para Python: acesso à CppNFe (NF-e) pela biblioteca nativa da CrossNFe.

Exemplo::

    from crossnfe import Session

    with Session() as session:
        root = session.root
        nota = root.notafiscal.NFe.add()
        nota.infNFe.det.add().prod.cProd = "001"
        root.notafiscal.execute("gerar")
        print(nota.XMLOriginal)

Os nomes de objetos e campos seguem a CppNFe, inclusive a capitalização.
"""
from ._native import CrossNFeError, Items, Model, Node, Session, library_path
from . import models

__version__ = "0.1.0"
__all__ = ["CrossNFeError", "Items", "Model", "Node", "Session", "library_path", "models", "__version__"]
