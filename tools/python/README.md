# CrossNFe para Python

Binding Python da CrossNFe, com exemplo de emissão e testes. Usa apenas a biblioteca padrão (`ctypes`) e
funciona no Windows x64 e no Linux x64 com Python 3.9 ou superior de 64 bits.

| Caminho | Conteúdo |
| --- | --- |
| `crossnfe/` | Pacote Python. `models.py` é gerado a partir dos cabeçalhos da CppNFe. |
| `emissao.py` | Exemplo de NF-e modelo 55, igual aos exemplos em C# e Java. |
| `config.properties` | Dados da nota e pasta de saída do exemplo. |
| `executar.sh`, `executar.ps1` | Executam o exemplo no Linux e no Windows. |
| `tests/` | Testes do binding (`unittest`). |
| `pyproject.toml` | Empacotamento do pacote `crossnfe`. |

## Executar o exemplo

```bash
# Linux
./executar.sh
./executar.sh --assinar-validar
```

```powershell
# Windows (PowerShell)
.\executar.ps1
.\executar.ps1 -AssinarValidar
```

Ou diretamente: `python emissao.py config.properties [--assinar-validar]`.

O exemplo preenche uma NF-e com dados fictícios, dois produtos e total de R$ 125,00. Ele gera o XML e confere
os dados por releitura. O fluxo local termina antes da transmissão à SEFAZ. Os XMLs ficam em `build/emissao`.

Para assinar e validar pelo XSD, defina `CROSSNFE_PFX`, `CROSSNFE_PFX_PASSWORD` e `CROSSNFE_SCHEMAS`. Use um
certificado de teste. As dependências nativas e a liberação do SHA1 no Fedora/RHEL estão descritas no
[README da pasta](../README.md#dependências-nativas).

## Biblioteca nativa

O pacote procura a biblioteca nesta ordem:

1. no arquivo indicado em `CROSSNFE_LIBRARY`;
2. na pasta do pacote `crossnfe` e nas duas pastas acima dela (por isso, nesta distribuição, encontra a
   `crossnfe.dll` ou a `libcrossnfe.so` da pasta `tools`);
3. pelo carregador do sistema.

No Windows, as dependências (`CppNFe.dll`, Qt, libxml2, OpenSSL) são procuradas na pasta da `crossnfe.dll` e
no `PATH`. No Linux, a `libCppNFe.so` é encontrada ao lado da `libcrossnfe.so`; o Qt 6.12+ deve estar no
sistema ou em `LD_LIBRARY_PATH` (o `executar.sh` usa `CROSSNFE_QT_LIB`).

## Usar em outro projeto

Instale o pacote e informe onde está a biblioteca nativa:

```bash
pip install ./tools/python
export CROSSNFE_LIBRARY=/caminho/libcrossnfe.so
```

```python
from crossnfe import CrossNFeError, Session

with Session() as session:
    root = session.root
    cfg = root.configuracoes
    cfg.ModeloDF = 55
    cfg.VersaoNF = 0            # VersaoNF::v400 (código do enum, não 400)
    cfg.webservices.tpAmb = 2   # homologação

    nota = root.notafiscal.NFe.add()
    det = nota.infNFe.det.add()
    det.prod.cProd = "001"
    det.prod.qCom = 2.0
    # ... demais grupos da nota

    try:
        root.notafiscal.execute("gerar")
    except CrossNFeError as erro:
        print(erro, nota.error)
    else:
        print(nota.chNFe, nota.XMLOriginal)
```

- `Session` deve ser usada com `with` (ou encerrada com `close()`) e sempre na thread que a criou. Crie a
  primeira sessão na thread principal.
- Os objetos obtidos pela sessão deixam de valer depois que ela é encerrada; `clear()` e `importar` invalidam os
  descendentes. Obtenha-os novamente pela raiz.
- Coleções: `add()`, `len()`, índice (inclusive negativo), fatias, iteração e `clear()`.
- Campos somente leitura, como `XMLOriginal`, levantam `AttributeError` na atribuição.
- Erros da biblioteca levantam `CrossNFeError`, com o código em `status`.
- Os tipos são anotados (`py.typed`), e os editores completam os nomes dos grupos e campos.

## Testes

```bash
python -m unittest discover -s tests -v
```

## Regenerar os modelos

`crossnfe/models.py` é gerado pelo `tools/generate.py` do projeto CrossNFe, junto com os modelos de C#, Java e
Delphi. Não edite o arquivo à mão.
