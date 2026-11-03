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

#### libxml2: recomendado 2.13 ou superior, e recompilar ao passar para a 2.14 ou superior
A lib foi testada com a libxml2 **2.12.10** e **2.15.4** (assinaturas idênticas e mesmo resultado nas validações por XSD).
- Na **2.13 ou superior**, erros em XSDs incluídos pelo schema principal (ex.: `leiauteNFe_v4.00.xsd` corrompido) vão para a
  mensagem de erro da validação. Na 2.12 ou anterior, também são impressos no stderr da aplicação.
- Na **2.14 ou superior**, a libxml2 mudou o nome do arquivo da biblioteca (`libxml2.so.2` → `libxml2.so.16`). Quem atualizar
  a libxml2 precisa **recompilar a CppBrasil e a aplicação**. Isso vale também para os novos pacotes de dependências do Windows.

### Adicionado
- Leiaute da NT 2023.004 v1.20: `Di::CPF`, `Icms::indDeduzDeson`
  (`IndDeduzDeson::None/Nao/Sim`), `Card::CNPJReceb` e `Card::idTermPag`, com
  leitura, escrita e limpeza dos campos. Geração de `indDeduzDeson` nos grupos
  ICMS20/30/40 (CST 40/41/50)/70/90, dentro dos dados de desoneração.
- Na NF-e/NFC-e, leitura e escrita de `DetPag::dPag`, `CNPJPag` e `UFPag`,
  já existentes para ECONF. Preencher CNPJPag/UFPag juntos; na DI, informar CNPJ
  ou CPF. O CNPJ do beneficiário da NF-e fica em `detPag->card->set_CNPJReceb(...)`,
  junto a `set_idTermPag(...)`; o ECONF conserva `DetPag::CNPJReceb/UFReceb`.
- `IndProc::CONFAZ` (4), `TpAto::AjusteSINIEF` (14) e `TpAto::ConvenioICMS` (15).
  Recompilar a biblioteca e as aplicações devido à ampliação das classes.
- Suporte a `Crt::SimplesMEI` (CRT 4) da NT 2024.001 v1.20, incluindo conversão
  na leitura e geração. Nos grupos `ICMSSN102` e `ICMSSN900`, a origem não preenchida
  (`Orig::None`) é omitida para MEI; origens informadas, inclusive nacional (0), são preservadas.
- Evento de Conciliação Financeira da NT 2024.002 v1.00, usando diretamente
  `EventoNotaFiscal::envEvento`, `TEvento` e `RetEnvEvento`. Inclui os eventos
  `TpEvento::ECONF` (110750) e `TpEvento::CancelECONF` (110751), os pagamentos em
  `DetEvento::detPag` (`Container<DetPag>`), geração e assinatura XML, schemas específicos
  de envio/retorno e direcionamento da NF-e para a SVRS e da NFC-e para a UF.
- `DetPag` ampliada com `dPag`, `CNPJPag`, `UFPag`, `CNPJReceb` e `UFReceb`,
  reutilizando `Card` para os dados da instituição de pagamento. Aplicações devem ser
  recompiladas devido à alteração do layout da classe.
- `Container::count()`, `isEmpty()`, `value(i)` (retorna `nullptr` para índice inválido) e `begin()`/`end()`,
  que permitem escrever `for (auto *x : *container)`.
- Valores padrão em `ConfigNFe`: `ModeloDF::NFe`, `VersaoNF::v400`, `VersaoQrCodeNF::v200`, `IndSinc::Nao` e `TpEmis::Normal`.
- Este arquivo de changelog.

### Corrigido
- Conversão de leitura dos CSTs do ICMS: agora usa a posição do enum, em vez do
  número fiscal, preservando também Part10/Part90/ST41/ST60. Antes a leitura podia
  resultar em CST incorreto ou acesso inválido ao gerar novamente.
- CST 41/50 passam a gerar o grupo compartilhado `ICMS40`, preservando o CST;
  ICMSPart10/90 usam `ICMSPart` (a identificação do grupo agora precede o corte do CST).
- Ato concessório 08 passa a ser escrito com dois dígitos; leitura aceita 08 e 8.
- CSOSN 103, 300 e 400 passam a usar o grupo XML `ICMSSN102`, conservando o valor de
  `CSOSN`. Antes eram gerados grupos inexistentes (`ICMSSN103`, `ICMSSN300` e `ICMSSN400`).
- Evento Ator Interessado na NF-e — Transportador (110150), conforme NT 2020.007 v1.40:
  reutiliza `EnvEvento`, `TEvento`, `DetEvento::autXML` e os enums existentes. Corrigida a tag
  `tpAutorizacao` e a emissão dos opcionais; valida as combinações de autor/autorização e
  restringe o evento à NF-e modelo 55. Preenche descrição, versão 1.00, órgão 91 e condição
  de uso quando autorizada. Direciona ao Ambiente Nacional em lote separado, usa os XSDs
  específicos de envio/retorno e gera XML processado para os status 135 e 136.
- `InfEvento::set_CPF()` gravava no membro CNPJ, impossibilitando informar corretamente
  o CPF do autor nos eventos.
- A associação de retorno e evento passa a conferir também `tpEvento`, além da chave e sequência.
- `EnvEvento::enviarEvento()` passa a retornar a falha do web service corretamente; a geração
  repetida limpa o XML do lote e o erro/assinatura anterior do evento.
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
- `CppXML::setNode` (`CppLibXml2`): valor com `&` ou `<` era tratado como XML já escapado. Na libxml2 2.12 a tag ficava
  vazia (e o erro saía no stderr); na 2.15 o `&` era descartado. Agora o valor é gravado como texto, escapado corretamente,
  igual nas duas versões. A assinatura não é afetada.
- `HttpQt`: o cabeçalho `Content-Type` era enviado com erro de digitação (`charst=utf-8`), e o charset era ignorado.
- Falha ao abrir o `urlnfe.json` interno agora gera um aviso claro, em vez de um erro enganoso de parse do JSON.
- Exemplo `consoleNFe`: usava `StatusServico::get_status()`, renomeado para `status()`.

### Alterado
- **Validação por XSD mais rápida** (`CppLibXml2::parseXML`): o schema era lido e interpretado a cada validação. Agora fica
  em cache no processo, compartilhado entre as threads (cerca de 11 vezes mais rápido: 1.600 validações de NF-e caíram de
  12 s para 1 s). Se algum `.xsd` da pasta for alterado, incluído ou removido, o schema é interpretado de novo
  automaticamente, sem reiniciar a aplicação. Os schemas usados ficam em memória até o fim do processo.
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

### Orientações de uso das Notas Técnicas

#### Evento ECONF (NT 2024.002 v1.00)
O acesso é feito por `cppnfe->eventos->envEvento`. Preencha os eventos em `evento->obj`
e confirme com `evento->add()`, usando `TpEvento::ECONF` ou `TpEvento::CancelECONF`.
Os pagamentos ficam em `evento->obj->infEvento->detEvento->detPag`, um `Container<DetPag>`.
O grupo reutiliza `DetPag` e seu objeto `card`: `card->set_CNPJ(...)` fornece `CNPJIF` no evento;
`card->set_tBand(...)` e `card->set_cAut(...)` fornecem os campos correspondentes,
serializados diretamente em `detPag` no XML de ECONF.
Recompile as aplicações após atualizar a biblioteca, pois `DetPag` recebeu novos membros.
Informe a versão do aplicativo em `detEvento->set_verAplic(...)` e, no cancelamento,
o protocolo do evento original em `detEvento->set_nProtEvento(...)`.
Envie com `cppnfe->eventos->envEvento->enviarEvento(idLote)` e consulte o retorno em
`cppnfe->eventos->envEvento->retorno`.

Configure a pasta de schemas com os XSDs de ECONF e cancelamento, incluindo seus arquivos
de dependência. No ambiente de desenvolvimento, eles estão em `build/schemas-nfe/`.
Envie conciliação e cancelamento em lotes separados, com até 20 eventos; cada ECONF aceita
de 1 a 100 pagamentos. A biblioteca preenche a descrição, versão do detalhe e órgão receptor:
NF-e usa SVRS (92); NFC-e usa o serviço da UF. No XSD fornecido, `CNPJIF`, `tBand` e `cAut`
dependem do preenchimento de `CNPJPag` e `UFPag`.

#### Ator interessado na NF-e — Transportador (NT 2020.007 v1.40)
Use `cppnfe->eventos->envEvento`, com `TpEvento::AtorInteressadoNFeTransp` (110150).
Preencha CNPJ **ou** CPF do autor, chave da NF-e modelo 55, data/hora e sequência de 1 a 20
em `evento->obj->infEvento`. Em `infEvento->detEvento`, informe:

- `set_cOrgaoAutor(...)`: código IBGE da UF do autor do evento;
- `set_tpAutor(...)`: `EmpresaEmitente` (1), `EmpresaDestinataria` (2) ou
  `EmpresaTerceiros` (3, transportador contratado);
- `set_verAplic(...)`: versão do aplicativo;
- `autXML->set_CNPJ(...)` **ou** `autXML->set_CPF(...)`: transportador autorizado;
- `set_tpAutorizacao(...)`: emitente/destinatário devem escolher `NaoPermite` (0) ou
  `PermiteTranspAutTransp` (1). Para transportador contratado, deixe `None`.

A biblioteca preenche descrição, versão do detalhe 1.00 e órgão receptor 91. Com autorização 1,
preenche também o texto oficial de `xCondUso` quando vazio. Nas demais opções, não informe esse texto.
Confirme cada evento com `evento->add()` e envie com `envEvento->enviarEvento(idLote)`;
o retorno fica em `envEvento->retorno`, incluindo `cOrgaoAutor`. Os status 135 e 136 geram
o XML `procEventoNFe`.

O envio usa exclusivamente o Ambiente Nacional, em lote separado dos demais tipos, com até
20 eventos. Configure os schemas `envEventoAtorInteressado_v1.00.xsd`,
`retEnvEventoAtorInteressado_v1.00.xsd` e suas dependências, incluindo
`leiauteEventoAtorInteressado_v1.00.xsd`. Para validar o XML processado, use
`procEventoAtorInteressado_v1.00.xsd`. Cópias locais para teste estão em `build/schemas-nfe/`.
A versão 1.40 da NT mantém o leiaute 1.00; a regra 4P24-10, sobre habilitação do autorizado
como emitente de CT-e, depende do cadastro consultado pela SEFAZ. O serviço também verifica
o prazo de seis meses e os demais requisitos fiscais do evento.

#### MEI — CRT 4 (NT 2024.001 v1.20)
Informe `emite->set_CRT(Crt::SimplesMEI)`. A leitura de `<CRT>4</CRT>` preserva esse regime.
Nos grupos `ICMSSN102` (CSOSN 102, 103, 300 ou 400) e `ICMSSN900`, `orig` é omitido
quando o emitente é MEI e `ICMS->get_orig()` vale `Orig::None`. Se informado, o valor é
preservado, inclusive `Orig::Nacional` (0). Nos demais regimes/grupos, a origem continua exigida.

Para MEI, `cEAN` e `cEANTrib` podem ficar vazios; as tags continuam presentes no XML.
Em operação interna, o NCM pode ser informado como `00000000`; nas operações interestaduais
e com o exterior, informe o NCM completo correto. O grupo `ICMSUFDest` não é exigido para CRT 4.
A biblioteca mantém os valores preenchidos pela aplicação, sem substituir NCM, CFOP, CSOSN
ou grupos de imposto automaticamente. Use schemas atualizados que aceitem CRT 4 e a origem
opcional nesses dois grupos.

A validação local por XSD verifica o leiaute. As regras fiscais da NT são aplicadas pela SEFAZ:
nas operações internas/interestaduais (`idDest != 3`), MEI pode usar CSOSN 102, 300, 400 e 900
na NF-e; na NFC-e, somente 102 e 300. Para CSOSN 102, os CFOPs de NF-e são 5102/6102;
para NFC-e, a regra do MEI permite somente 5102. CSOSN 900 e devoluções têm listas próprias
de CFOP na NT, que devem ser observadas pela aplicação.

A NT também substitui denegações por rejeições (302, 303 e 307) e exclui a denegação 301;
os códigos e motivos recebidos continuam disponíveis nos retornos da biblioteca.

#### Informações de pagamentos e outros — NT 2023.004 v1.20

`validar()` verifica o XSD; as regras fiscais desta NT são avaliadas pela SEFAZ.
A biblioteca conserva os valores informados pela aplicação, sem recalcular totais
ou criar automaticamente grupos de pagamento.

- A versão 1.20 aumenta o limite fiscal do troco para **R$ 300.000,00** (YA09-20).
  A biblioteca já permitia representar esse valor, sem limite local de R$ 1.000,00.
- `indDeduzDeson`: ausência (`None`) e 0 (`Nao`) não exigem subtrair a desoneração
  do total, conforme a exceção 4 de W16-10. Com 1 (`Sim`), a aplicação deve compor
  os totais considerando a dedução. Para motivo 7 (SUFRAMA), N28-12 exige 1.
- `CNPJPag`/`UFPag` identificam o estabelecimento onde o pagamento foi processado;
  `card/CNPJReceb` identifica o beneficiário e `idTermPag` o terminal. `cAut` aceita
  autorização de cartões, PIX, boletos e outros pagamentos eletrônicos, até 128 caracteres.
- YA04-20 permite `card` para tPag 03/04/10/11/12/13/15/17/18. A exigência de
  `card` para cartão/PIX na NFC-e (YA04-10) depende da UF. Preencher `tpIntegra`
  quando houver `card`; a biblioteca não inventa os dados de integração.
- A aplicação deve informar data de pagamento válida e documentos com dígitos
  verificadores válidos. Essas verificações fiscais não são feitas pelo XSD.
- X03-10/X03-20 foram desabilitadas pela NT; não há bloqueio local correspondente.
  A exceção de NFA-e à habilitação do emitente (1C17-34) depende da SEFAZ.
- Utilizar schemas atualizados. O ECONF foi transferido para a NT 2024.002,
  implementada anteriormente.
