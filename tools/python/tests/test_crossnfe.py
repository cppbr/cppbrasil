"""Testes do binding Python. Execute na pasta python: python -m unittest discover -s tests"""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from crossnfe import CrossNFeError, Items, Session, library_path  # noqa: E402
from crossnfe import models  # noqa: E402


class CrossNFeTest(unittest.TestCase):
    def test_biblioteca_encontrada(self):
        self.assertIsNotNone(library_path())

    def test_propriedades_ida_e_volta(self):
        with Session() as s:
            prod = s.root.notafiscal.NFe.add().infNFe.det.add().prod
            prod.cProd = "ÁÇÃ-001"
            prod.qCom = 2.5
            prod.nItem = 7
            prod.indTot = 1
            self.assertEqual(prod.cProd, "ÁÇÃ-001")
            self.assertEqual(prod.qCom, 2.5)
            self.assertEqual(prod.nItem, 7)
            cfg = s.root.configuracoes.arquivos
            cfg.salvar = True
            self.assertTrue(cfg.salvar)
            cfg.salvar = False
            self.assertFalse(cfg.salvar)

    def test_colecoes(self):
        with Session() as s:
            det = s.root.notafiscal.NFe.add().infNFe.det
            self.assertIsInstance(det, Items)
            for codigo in ("A", "B", "C"):
                det.add().prod.cProd = codigo
            self.assertEqual(len(det), 3)
            self.assertEqual([d.prod.cProd for d in det], ["A", "B", "C"])
            self.assertEqual(det[-1].prod.cProd, "C")
            self.assertEqual([d.prod.cProd for d in det[1:]], ["B", "C"])
            self.assertEqual(det[0], det[0])
            self.assertIsInstance(det[0], models.Det)
            with self.assertRaises(CrossNFeError):
                det[10]
            det.clear()
            self.assertEqual(len(det), 0)

    def test_aninhadas(self):
        with Session() as s:
            prod = s.root.notafiscal.NFe.add().infNFe.det.add().prod
            prod.DI.add().adi.add().nAdicao = 3
            self.assertEqual(prod.DI[0].adi[0].nAdicao, 3)

    def test_somente_leitura_e_tipos(self):
        with Session() as s:
            nota = s.root.notafiscal.NFe.add()
            with self.assertRaises(AttributeError):
                nota.XMLOriginal = "x"
            with self.assertRaises(TypeError):
                nota.infNFe.det.add().prod.cProd = 10
            with self.assertRaises(ValueError):
                nota.infNFe.det.add().prod.cProd = "a\0b"

    def test_sessao_encerrada(self):
        s = Session()
        nota = s.root.notafiscal.NFe.add()
        s.close()
        self.assertTrue(s.closed)
        with self.assertRaises(CrossNFeError):
            nota.chNFe
        s.close()  # idempotente

    def test_importar_xml_invalido(self):
        with Session() as s:
            with self.assertRaises(CrossNFeError):
                s.root.notafiscal.execute("importar", "<nao-e-nfe/>")


if __name__ == "__main__":
    unittest.main()
