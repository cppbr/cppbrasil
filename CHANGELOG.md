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
- Exemplo `consoleNFe`: usava `StatusServico::get_status()`, renomeado para `status()`.

### Alterado
- Objetos temporários internos (`CppLibXml2`, web services, `XmlRead`, cliente HTTP, ...) passaram a usar
  `std::unique_ptr`, sem vazamento de memória em caso de exceção.
- `Container::add()` ficou seguro contra exceção.
- O layout interno de `CppDanfeQtRPT` mudou: as aplicações que usam a lib do DANFE precisam ser recompiladas.

### Removido
- Dependências pré-compiladas antigas para Windows em `third-party/windows/` (OpenSSL 3.1.3, libxml2 2.11.5,
  zlib 1.3 e libiconv). As versões novas serão distribuídas como pacotes compactados. Até lá, o `installer/MakeMSVC.bat`
  e os `CMakeLists.txt` dos exemplos apontam para caminhos que não existem mais.
- Exemplo `examples/QtCon-2022` (legado, sem manutenção).
