# Changelog

Todas as mudanças relevantes do projeto são registradas aqui.
O formato segue o [Keep a Changelog](https://keepachangelog.com/pt-BR/1.1.0/).

## [Não lançado]

### ⚠️ Mudanças que exigem atenção (quebra de compatibilidade)

#### Membros das classes da NFe não podem mais ser copiados
Os membros da árvore da NFe (`CppNFe`, `NotaFiscal`, `Nfe`, `InfNFe`, `Det`, `Prod`, ...) passaram de
`std::shared_ptr` para `std::unique_ptr`. O acesso pelo `->` **não muda**, mas copiar um membro deixa de compilar.

```cpp
// Antes (não compila mais)
auto nf = cppnfe->notafiscal;
std::shared_ptr<NotaFiscal> nf = cppnfe->notafiscal;

// Agora
auto &nf = cppnfe->notafiscal;               // referência
NotaFiscal *nf = cppnfe->notafiscal.get();   // ponteiro (não dono)

// Continua igual
cppnfe->notafiscal->NFe->obj->infNFe->ide->set_nNF(1);
```

#### `Container`: acesso direto a `items` está obsoleto (deprecated)
O `Container` ganhou uma API própria de leitura. `items` continua funcionando nesta versão, mas gera aviso
de compilação e **será removido numa versão futura**.

```cpp
// Antes (gera aviso de deprecated)
for (int i = 0; i < nfe->infNFe->det->items->count(); ++i)
    nfe->infNFe->det->items->value(i)->prod->get_cProd();

// Agora
for (int i = 0; i < nfe->infNFe->det->count(); ++i)
    nfe->infNFe->det->value(i)->prod->get_cProd();

// ou, com for por intervalo
for (Det *det : *nfe->infNFe->det)
    det->prod->get_cProd();
```

A **inserção de dados não muda**: continue preenchendo `obj` e chamando `add()`.

#### `Container::add(T*)` passou a receber `std::unique_ptr<T>`
Isso deixa explícito que o container vira o dono do objeto.

```cpp
// Antes
container->add(new Nfe);
// Agora
container->add(std::make_unique<Nfe>());
```

#### `Container` não pode mais ser copiado
A cópia causava double delete (crash). Agora gera erro de compilação.

#### `gerar()` valida a chave de acesso
Se a chave de acesso montada (sem o dígito verificador) não tiver exatamente 43 dígitos numéricos, `gerar()`
passa a devolver `false` com a mensagem `Chave de acesso invalida (...)`. Antes, a nota era gerada em silêncio
com uma chave que a SEFAZ rejeitaria. A causa mais comum é o **CNPJ/CPF do emitente com máscara**
(`12.345.678/0001-90`): informe somente os números.

#### OpenSSL 1.1.1 ou superior e leitura do `openssl.cnf`
A lib passa a exigir o **OpenSSL 1.1.1 ou superior** (com versões antigas, a compilação para com uma mensagem clara)
e a **carregar o arquivo de configuração do OpenSSL** (`openssl.cnf` do sistema ou o indicado em `OPENSSL_CONF`).
Antes, a configuração era ignorada. Com isso, a política de criptografia do sistema passa a valer: no Fedora/RHEL
e derivados, é preciso liberar a assinatura SHA1 (ver README).

### Adicionado
- `Container::count()`, `isEmpty()`, `value(i)` (retorna `nullptr` para índice inválido) e `begin()`/`end()`,
  que permitem escrever `for (auto *x : *container)`.
- Valores padrão em `ConfigNFe`: `ModeloDF::NFe`, `VersaoNF::v400`, `VersaoQrCodeNF::v200`, `IndSinc::Nao` e `TpEmis::Normal`.
- Este arquivo de changelog.

### Corrigido
- **XML de NF-e com DI inválido**: a tag `UFDesemb` era gerada duas vezes no grupo `DI`, e a SEFAZ rejeitava
  notas de importação.
- **XML duplicado ao gerar de novo**: `gerar()` numa nota importada, ou chamado mais de uma vez, produzia um XML
  com duas raízes `<NFe>`. Agora o XML, a assinatura e o erro anteriores são limpos antes de gerar.
- `gerar()` devolvia `false` quando havia um erro de uma etapa anterior (por exemplo, de `validar()`), mesmo gerando corretamente.
- `gerar()` devolvia `true` quando a geração do XML falhava. Agora o erro é informado.
- A mensagem "Erro ao assinar/gerar/validar N nota(s)" mostrava a contagem anterior (por exemplo, "0 nota(s)").
- **Configuração sem valor inicial**: membros de `ConfigNFe`, `ArquivosNF` e `ConfWebServices` não eram
  inicializados. Sem chamar os `set_...`, eles ficavam com lixo de memória, o que podia, por exemplo, quebrar a geração do QR Code da NFC-e.
- Acesso fora dos limites da string ao montar a versão do QR Code (online e offline).
- DANFE: o objeto do relatório tinha dois donos, e cada chamada de `print()` no mesmo objeto duplicava as conexões
  (os campos eram preenchidos várias vezes).
- DANFE: a lista de locais de retirada e entrega era global e compartilhada entre todas as instâncias. Duas impressões
  ao mesmo tempo podiam misturar os dados. Agora cada DANFE tem a sua lista, e campos numéricos sem valor saem zerados
  em vez de lixo de memória.
- **Dígito verificador** (`CppUtility::dvModulo11`): sempre lia 43 posições, sem considerar o tamanho da string.
  Na chave de contingência FS/FS-DA do DANFE (35 posições), lia além do fim da string, e o DV podia sair errado.
  Agora o cálculo usa o tamanho real. Para chaves de 43 dígitos, o resultado não muda.
- DANFE: a chave de contingência FS/FS-DA saía inválida para notas a partir de R$ 10.000,00, porque o valor
  era formatado em notação científica (ex.: `1e+06`).
- A assinatura alterava configurações globais da libxml2, afetando todos os parses seguintes (validação por XSD e
  retornos da SEFAZ). As opções agora valem só para a assinatura. A assinatura gerada não muda.
- **Assinatura no Fedora/RHEL** (e derivados): falhava sempre com "Erro ao assinar documento", porque a lib ignorava
  a configuração do OpenSSL e o SHA1 ficava bloqueado sem como liberar. Agora basta liberar o SHA1 (ver README).
- **`HttpOpenSSL`** (usado com `HttpType::HttpOpenSSL`; o padrão `HttpQt` não é afetado):
  - a resposta vinha com lixo de memória depois do XML, e respostas grandes podiam vir cortadas. Agora a leitura
    respeita o `Content-Length`, decodifica `Transfer-Encoding: chunked` e aceita servidor que fecha sem `close_notify`;
  - não tinha timeout e podia travar a aplicação. Agora usa o timeout configurado (padrão de 15 s, como o `HttpQt`);
  - XML com acentos podia ser corrompido no Windows (conversão para a codificação local) e o `Content-Length` ficava errado;
  - linha do pedido HTTP inválida (espaço antes do fim de linha), SNI não enviado e `SSL_accept` indevido no cliente;
  - socket, `SSL_CTX` e `SSL` vazavam nos retornos de erro.
- **Crash na assinatura** (`CppLibXml2::c14n`): quando o nó a assinar não existia no XML, a lib liberava a raiz do documento
  duas vezes (`double free`) e a aplicação fechava. Agora a assinatura falha com erro.
- A assinatura chamava `xmlCleanupParser()`, que limpa o estado global da libxml2 e não deve ser chamada com a biblioteca em uso
  (risco com threads ou com outras partes da aplicação que usam a libxml2).
- Validação por XSD (`parseXML`): os erros da libxml2 eram impressos no stderr da aplicação e a mensagem de erro terminava com
  quebra de linha. Agora os erros são capturados pela lib, e a mensagem informa o **primeiro** erro do XML (antes era o último).
  Diretório de schemas inválido encerra a validação na hora.
- **Assinatura com DOCTYPE** (`signXML`): um XML com `<!DOCTYPE>` e entidade externa fazia o conteúdo de arquivos locais
  entrar no XML assinado (XXE), e podia acessar a rede. Agora XML com DOCTYPE é recusado na assinatura (o XML da NF-e
  não tem DOCTYPE; a assinatura das notas não muda).
- `HttpQt`: o cabeçalho `Content-Type` era enviado com erro de digitação (`charst=utf-8`), e o charset era ignorado.
- Falha ao abrir o `urlnfe.json` interno agora gera um aviso claro, em vez de um erro enganoso de parse do JSON.
- Exemplo `consoleNFe`: usava `StatusServico::get_status()`, renomeado para `status()`.

### Alterado
- `CppLibXml2`: troca as funções antigas `xmlParseMemory`/`xmlParseFile` (interface SAX1, que pode estar desligada na
  compilação da libxml2) por `xmlReadMemory`/`xmlReadFile`. A lib compila com libxml2 sem SAX1 e não acessa a rede ao ler XML.
  Os erros de leitura não vão mais para o stderr, e a mensagem de XML malformado informa o motivo
  (ex.: `Erro: Falha ao interpretar o XML: 77 - Premature end of data in tag vBC line 1`).
- Objetos temporários internos (`CppLibXml2`, web services, `XmlRead`, cliente HTTP, ...) passaram a usar
  `std::unique_ptr`, sem vazamento de memória em caso de exceção.
- `Container::add()` ficou seguro contra exceção.
- O layout interno de `CppDanfeQtRPT` mudou: as aplicações que usam a lib do DANFE precisam ser recompiladas.

### Removido
- Dependências pré-compiladas antigas para Windows em `third-party/windows/` (OpenSSL 3.1.3, libxml2 2.11.5,
  zlib 1.3 e libiconv). As versões novas serão distribuídas como pacotes compactados. Até lá, o `installer/MakeMSVC.bat`
  e os `CMakeLists.txt` dos exemplos apontam para caminhos que não existem mais.
- Exemplo `examples/QtCon-2022` (legado, sem manutenção).
