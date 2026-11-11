# CrossNFe

A CrossNFe é a interface que permite usar a CppNFe em C#, Java e outras linguagens capazes de chamar uma DLL
nativa. O núcleo fiscal continua na CppNFe. A aplicação navega pelos mesmos objetos e coleções do C++, com
tipagem em cada linguagem.

Versão 0.1.0, ABI 1. Binários para Windows x64.

## Conteúdo da pasta

| Caminho | Conteúdo |
| --- | --- |
| `crossnfe.dll` | Biblioteca nativa da CrossNFe (MSVC, Release). |
| `CppNFe.dll` | Biblioteca da CppBrasil usada pela `crossnfe.dll`. Já inclui a base comum (`CppBase`). |
| [`csharp/`](csharp/README.md) | Exemplo em C# (.NET 10) com o binding `CrossNFe.Managed.dll`. |
| [`java/`](java/README.md) | Exemplo em Java (JDK 22+, API FFM) com o binding `crossnfe.jar`. |

Cada exemplo traz a própria cópia da `crossnfe.dll` e as licenças em `licencas/`.

## Início rápido

1. Instale o SDK .NET 10 x64 (C#) ou um JDK 22+ de 64 bits (Java).
2. Deixe as dependências nativas da tabela abaixo no `PATH` ou ao lado da `crossnfe.dll`.
3. Dentro da pasta do exemplo, execute no PowerShell:

```powershell
.\executar.ps1                  # gera o XML e confere por releitura
.\executar.ps1 -AssinarValidar  # também assina e valida pelo XSD
```

Para assinar e validar, defina antes as variáveis de ambiente:

| Variável | Conteúdo |
| --- | --- |
| `CROSSNFE_PFX` | Caminho do certificado A1 (`.pfx`). Use um certificado de teste. |
| `CROSSNFE_PFX_PASSWORD` | Senha do certificado. |
| `CROSSNFE_SCHEMAS` | Pasta dos schemas da NF-e, com `nfe_v4.00.xsd` e os demais arquivos do pacote. |

Os exemplos usam dados fictícios em homologação e terminam antes da transmissão: nenhum documento é enviado à
SEFAZ. Os XMLs ficam em `build/emissao`, dentro da pasta do exemplo.

## Dependências nativas

Todas devem ser Windows x64 e compatíveis com a compilação MSVC da `crossnfe.dll`.

| Componente | DLLs |
| --- | --- |
| CppBrasil | `CppNFe.dll` (nesta pasta) |
| Qt 6 (kit `msvc2022_64`) | `Qt6Core.dll`, `Qt6Network.dll` |
| libxml2 (MSVC) | `libxml2.dll` |
| OpenSSL 3 (3.5.9) | `libcrypto-3-x64.dll`, `libssl-3-x64.dll` |
| Microsoft Visual C++ Redistributable x64 | `MSVCP140.dll`, `MSVCP140_1.dll`, `VCRUNTIME140.dll`, `VCRUNTIME140_1.dll` |

A partir desta versão não existe mais a `CppBase.dll`: a base comum faz parte da `CppNFe.dll`.

## Uso da API

Uma `Session` cria uma instância da CppNFe. Pela raiz, você chega a `configuracoes`, a `notafiscal` e às
coleções da nota. As operações são chamadas com `Execute`:

| Objeto | Operação | Efeito |
| --- | --- | --- |
| `notafiscal` | `gerar` | Monta o XML das notas da coleção `NFe` (`XMLOriginal`, `chNFe`). |
| `notafiscal` | `assinar` | Assina o XML com o certificado carregado (`XMLAssinado`). |
| `notafiscal` | `validar` | Valida o XML assinado pelos schemas configurados. |
| `notafiscal` | `importar` | Lê um XML de NF-e para a coleção `NFe`. |
| `configuracoes.certificado` | `carregar` | Carrega o certificado. O argumento é um JSON `{"arquivo": ..., "senha": ...}`. |

Se uma operação falhar, a exceção traz a mensagem, e o campo `error` da nota traz o detalhe.

### C#

```csharp
using CrossNFe;

using var session = new Session();
var root = session.Root;

var cfg = root.configuracoes;
cfg.ModeloDF = 55;
cfg.VersaoNF = 0;           // VersaoNF::v400 (código do enum, não 400)
cfg.webservices.tpAmb = 2;  // homologação

var nota = root.notafiscal.NFe.Add();
var produto = nota.infNFe.det.Add().prod;
produto.cProd = "001";
produto.xProd = "Produto";
// ... demais grupos da nota

root.notafiscal.Execute("gerar");
string xml = nota.XMLOriginal;
```

### Java

```java
import crossnfe.Session;

try (var session = new Session()) {
    var root = session.root();
    var nota = root.notafiscal().NFe().add();
    // ... preencher a nota e as configurações
    root.notafiscal().execute("gerar", "");
    String xml = nota.get_XMLOriginal();
}
```

Inicie a JVM com `--enable-native-access=ALL-UNNAMED`, e informe o caminho da DLL com
`-Dcrossnfe.library=<pasta>\crossnfe.dll`. O `executar.ps1` já faz isso.

## Convenções

- Os nomes dos objetos e campos seguem a CppNFe, inclusive a capitalização (`infNFe`, `det`, `prod`, `cProd`).
- Datas são strings ISO 8601, por exemplo `2026-11-10T12:00:00-04:00`.
- Os campos monetários e as quantidades usam `double`, como na CppNFe.
- Os enums usam os códigos inteiros do C++, que podem diferir do valor gravado no XML. Consulte
  [convdf.h](../src/CppBase/convdf.h) e [convnf.h](../src/CppNFe/convnf.h).
- Em Java, os campos são acessados por `get_<campo>()` e `set_<campo>(...)`, e os objetos e coleções por
  métodos com o nome do grupo (`infNFe()`, `NFe()`).

## Limitações

- Somente Windows x64 (MSVC).
- Quatro propriedades ainda ficam fora da interface: `ConfWebServices.protocoloSSL`, `ConfWebServices.proxy`,
  `GCompraGov.refDFeAnt` e `GPagAntecipado.refNFe`.
- Os exemplos cobrem geração, releitura, assinatura e validação. Transmissão, consulta, cancelamento e
  inutilização usam a mesma API, mas não fazem parte dos exemplos. Veja o exemplo `consoleNFe` em C++, em
  [`examples/`](../examples).

## Licenças

A CppBrasil e a CrossNFe são distribuídas sob a Apache License 2.0. O Qt segue a licença do próprio Qt. Os
textos completos estão na pasta `licencas/` de cada exemplo.
