# CrossNFe

A CrossNFe é a interface que permite usar a CppNFe em C#, Java, Python e outras linguagens capazes de chamar
uma biblioteca nativa. O núcleo fiscal continua na CppNFe. A aplicação navega pelos mesmos objetos e coleções
do C++, com tipagem em cada linguagem.

Versão 0.1.0, ABI 1. Binários para Windows x64 e Linux x64.

## Conteúdo da pasta

| Caminho | Conteúdo |
| --- | --- |
| `crossnfe.dll` | Biblioteca nativa da CrossNFe para Windows x64 (MSVC, Release). |
| `CppNFe.dll` | Biblioteca da CppBrasil usada pela `crossnfe.dll`. Já inclui a base comum (`CppBase`). |
| `libcrossnfe.so` | Biblioteca nativa da CrossNFe para Linux x64 (GCC, Release). |
| `libCppNFe.so` | Biblioteca da CppBrasil usada pela `libcrossnfe.so`. Já inclui a base comum (`CppBase`). |
| [`csharp/`](csharp/README.md) | Exemplo em C# (.NET 10) com o binding `CrossNFe.Managed.dll`. |
| [`java/`](java/README.md) | Exemplo em Java (JDK 22+, API FFM) com o binding `crossnfe.jar`. |
| [`python/`](python/README.md) | Binding Python (`crossnfe`, ctypes) com exemplo e testes. |

No Windows, os exemplos em C# e Java trazem a própria cópia da `crossnfe.dll`. No Linux, todos usam as
bibliotecas desta pasta. Cada exemplo traz as licenças em `licencas/`.

## Início rápido

1. Instale o SDK .NET 10 x64 (C#), um JDK 22+ de 64 bits (Java) ou o Python 3.9+ de 64 bits (Python).
2. Deixe as dependências nativas da tabela abaixo acessíveis ao carregador do sistema.
3. Dentro da pasta do exemplo, execute:

```powershell
# Windows (PowerShell)
.\executar.ps1                  # gera o XML e confere por releitura
.\executar.ps1 -AssinarValidar  # também assina e valida pelo XSD
```

```bash
# Linux
./executar.sh                    # gera o XML e confere por releitura
./executar.sh --assinar-validar  # também assina e valida pelo XSD
```

Para assinar e validar, defina antes as variáveis de ambiente:

| Variável | Conteúdo |
| --- | --- |
| `CROSSNFE_PFX` | Caminho do certificado A1 (`.pfx`). Use um certificado de teste. |
| `CROSSNFE_PFX_PASSWORD` | Senha do certificado. |
| `CROSSNFE_SCHEMAS` | Pasta dos schemas da NF-e, com `nfe_v4.00.xsd` e os demais arquivos do pacote. |
| `CROSSNFE_QT_LIB` | Somente Linux: pasta `lib` do Qt 6.12+, se o Qt do sistema for mais antigo. |

Os exemplos usam dados fictícios em homologação e terminam antes da transmissão: nenhum documento é enviado à
SEFAZ. Os XMLs ficam em `build/emissao`, dentro da pasta do exemplo. Os três exemplos geram o mesmo XML.

## Dependências nativas

### Windows x64

Todas devem ser compatíveis com a compilação MSVC da `crossnfe.dll`. Deixe-as no `PATH` ou ao lado da
`crossnfe.dll`.

| Componente | DLLs |
| --- | --- |
| CppBrasil | `CppNFe.dll` (nesta pasta) |
| Qt 6 (kit `msvc2022_64`) | `Qt6Core.dll`, `Qt6Network.dll` |
| libxml2 (MSVC) | `libxml2.dll` |
| OpenSSL 3 (3.5.9) | `libcrypto-3-x64.dll`, `libssl-3-x64.dll` |
| Microsoft Visual C++ Redistributable x64 | `MSVCP140.dll`, `MSVCP140_1.dll`, `VCRUNTIME140.dll`, `VCRUNTIME140_1.dll` |

A partir desta versão não existe mais a `CppBase.dll`: a base comum faz parte da `CppNFe.dll`.

### Linux x64

A `libcrossnfe.so` encontra a `libCppNFe.so` na mesma pasta (RUNPATH `$ORIGIN`). As demais bibliotecas vêm do
sistema ou do `LD_LIBRARY_PATH`.

| Componente | Bibliotecas |
| --- | --- |
| CppBrasil | `libCppNFe.so` (nesta pasta) |
| Qt 6.12 ou superior | `libQt6Core.so.6`, `libQt6Network.so.6` |
| libxml2 | `libxml2.so.2` |
| OpenSSL 3 | `libssl.so.3`, `libcrypto.so.3` |
| zlib | `libz.so.1` |

As bibliotecas foram compiladas com o Qt 6.12 e exigem Qt 6.12 ou superior em tempo de execução. Se o Qt da
distribuição for mais antigo, instale o Qt pelo instalador oficial e informe a pasta `lib` em `CROSSNFE_QT_LIB`
(os scripts `executar.sh` a acrescentam ao `LD_LIBRARY_PATH`). Para usar o Qt da distribuição, recompile a
CppNFe e a CrossNFe com ele.

No Fedora, RHEL e derivados, o OpenSSL bloqueia assinaturas RSA-SHA1, usadas pela NF-e, e a assinatura falha com
"Erro ao assinar documento". Libere o SHA1 no sistema (`sudo update-crypto-policies --set DEFAULT:SHA1`) ou só
na aplicação, apontando `OPENSSL_CONF` para um arquivo como o descrito no README principal da CppBrasil.

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

Inicie a JVM com `--enable-native-access=ALL-UNNAMED` e informe o caminho da biblioteca com
`-Dcrossnfe.library=<pasta>\crossnfe.dll` (Windows) ou `-Dcrossnfe.library=<pasta>/libcrossnfe.so` (Linux). Os
scripts `executar.ps1` e `executar.sh` já fazem isso.

### Python

```python
from crossnfe import Session

with Session() as session:
    root = session.root
    root.configuracoes.ModeloDF = 55
    nota = root.notafiscal.NFe.add()
    produto = nota.infNFe.det.add().prod
    produto.cProd = "001"
    # ... preencher a nota e as configurações
    root.notafiscal.execute("gerar")
    xml = nota.XMLOriginal
```

O pacote procura a biblioteca nativa em `CROSSNFE_LIBRARY`, na própria pasta e nas duas pastas acima dela. Veja
[python/README.md](python/README.md).

## Convenções

- Os nomes dos objetos e campos seguem a CppNFe, inclusive a capitalização (`infNFe`, `det`, `prod`, `cProd`).
- Datas são strings ISO 8601, por exemplo `2026-11-10T12:00:00-04:00`.
- Os campos monetários e as quantidades usam `double` (`float` em Python), como na CppNFe.
- Os enums usam os códigos inteiros do C++, que podem diferir do valor gravado no XML. Consulte
  [convdf.h](../src/CppBase/convdf.h) e [convnf.h](../src/CppNFe/convnf.h).
- Em Java, os campos são acessados por `get_<campo>()` e `set_<campo>(...)`, e os objetos e coleções por
  métodos com o nome do grupo (`infNFe()`, `NFe()`).
- Em Python, os campos e grupos são atributos (`nota.infNFe.det`), e as coleções aceitam `len()`, índice
  (inclusive negativo), fatias e iteração.

## Limitações

- Windows x64 (MSVC) e Linux x64.
- Quatro propriedades ainda ficam fora da interface: `ConfWebServices.protocoloSSL`, `ConfWebServices.proxy`,
  `GCompraGov.refDFeAnt` e `GPagAntecipado.refNFe`.
- Os exemplos cobrem geração, releitura, assinatura e validação. Transmissão, consulta, cancelamento e
  inutilização usam a mesma API, mas não fazem parte dos exemplos. Veja o exemplo `consoleNFe` em C++, em
  [`examples/`](../examples).

## Licenças

A CppBrasil e a CrossNFe são distribuídas sob a Apache License 2.0. O Qt segue a licença do próprio Qt. Os
textos completos estão na pasta `licencas/` de cada exemplo.
