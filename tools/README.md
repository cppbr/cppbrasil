# CrossNFe

Interface entre a CppNFe e aplicações C#, Java, Python e outras linguagens. O núcleo fiscal permanece no projeto CppBrasil e a aplicação navega por objetos e coleções tipadas. Versão inicial 0.1.0, ABI 1.

## Uso em `C#`

```csharp
using var session = new CrossNFe.Session();
var notas = session.Root.notafiscal.NFe;
var nota = notas.Add();
var produto = nota.infNFe.det.Add().prod;
produto.cProd = "001";
produto.xProd = "Produto";
produto.DI.Add().adi.Add().nAdicao = 1;
// Preencher os demais dados fiscais e configurar antes de gerar:
// session.Root.notafiscal.Execute("gerar");
// string xml = nota.XMLOriginal;
```

Os nomes dos objetos e campos seguem o núcleo, inclusive a capitalização. Datas são strings ISO 8601 e campos monetários usam `double`, como na CppNFe. Os enums usam os códigos inteiros do C++, que podem diferir dos valores serializados no XML. Consulte [convdf.h](../src/CppBase/convdf.h) e [convnf.h](../src/CppNFe/convnf.h).

Esta versão deixa quatro propriedades fora da interface (`ConfWebServices.protocoloSSL`, `ConfWebServices.proxy`, `GCompraGov.refDFeAnt` e `GPagAntecipado.refNFe`).

## SDKs e exemplos

- [Exemplo em C#](csharp/README.md) com .NET 10 e o binding `CrossNFe.Managed.dll`.
- [Exemplo em Java](java/README.md) com JDK 22+, API FFM e o binding `crossnfe.jar`.

Os dois exemplos incluem a `crossnfe.dll` para Windows x64. Cada README descreve as dependências nativas e como gerar, reler, assinar e validar o XML.
