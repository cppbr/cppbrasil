# Changelog

Todas as mudanças relevantes do projeto são registradas neste arquivo.
O formato segue o [Keep a Changelog](https://keepachangelog.com/pt-BR/1.1.0/).
As alterações ainda sem versão de lançamento estão agrupadas em **Não publicado**.

## [Não publicado]

### Adicionado

#### Reforma Tributária: IBS, CBS e IS no XML da NF-e e NFC-e — NT 2025.002 v1.53

- Informe em `Ide` a data de previsão de entrega (`dPrevEntrega`, somente
  NF-e modelo 55), o município do fato gerador do IBS/CBS (`cMunFGIBS`),
  as finalidades `FinNFe::NotaCredito` e `FinNFe::NotaDebito` com
  `tpNFDebito` ou `tpNFCredito` e o indicador do local da operação
  (`cIndOp`). Os grupos `gCompraGov` (compras governamentais) e
  `gPagAntecipado` (notas de antecipação de pagamento) ficam em `Ide`.
- Informe `ISUFEmit` em `Emit` e `tpCredPresIBSZFM` e `indBemMovelUsado`
  em `Prod`.
- Preencha o Imposto Seletivo em `Imposto::IS` e o IBS/CBS em
  `Imposto::IBSCBS`. Cada grupo do leiaute tem objeto próprio, com os nomes
  do XML: `gIBSCBS` (com `gIBSUF`, `gIBSMun`, `gCBS`, `gDif`, `gDevTrib`,
  `gRed`, `gALCZFMCBS`, `gTribRegular` e `gTribCompraGov`), `gIBSCBSMono`
  no leiaute de monofasia da v1.50 (AdRem ou AdValorem por tributo),
  `gTransfCred`, `gAjusteCompet`, `gEstornoCred`, `gCredPresOper` e
  `gCredPresIBSZFM`.
- Os grupos são gerados quando preenchidos. Dentro de um grupo emitido, os
  campos obrigatórios saem mesmo com valor zero. A geração recusa a nota
  quando o item informa mais de um grupo mutuamente exclusivo, como
  `gIBSCBS` e `gTransfCred`, ou valores do IBS/CBS sem o `CST`.
- Informe `vItem` e `DFeReferenciado` em `Det` e os totais `ISTot`,
  `IBSCBSTot` e `vNFTot` em `Total`. Os totais são preenchidos pela
  aplicação, como os demais totais da nota.
- A leitura do XML importa todos os campos e grupos acima. Recompile a
  biblioteca e as aplicações devido às alterações em `Ide`, `Emit`,
  `Prod`, `Det`, `Imposto` e `Total`.

#### Inutilização de numeração de NF-e e NFC-e

- `EventoNotaFiscal::inutNFe` disponibiliza `InutNFe`, `InfInut` e
  `RetInutNFe`, com campos encapsulados. Pedido e retorno usam instâncias
  distintas de `InfInut`, reunindo os campos dos dois grupos.
- Use `inutNFe->infInut->inutilizar()` para gerar, assinar, validar e enviar
  o pedido pelo serviço próprio de inutilização, de forma síncrona e
  independente do lote de eventos. Consulte o resultado em
  `inutNFe->retInutNFe->infInut`.
- O envio valida ano, série, justificativa e faixa de até 10.000 números.
  A confirmação `cStat=102` gera o XML processado após conferir os dados
  do pedido assinado, preservando a assinatura opcional do retorno.
  Rejeições ficam disponíveis para consulta, incluindo o protocolo
  anterior quando informado na duplicidade `563`.

#### XML da NF-e com DANFE Simplificado Tipo 2 — NT 2026.002 v1.11

- Use `TpImp::SimplificadoTipo2` (`tpImp=6`) na NF-e modelo 55.
  A geração usa QR Code v3 e permite omitir o destinatário nas operações
  presenciais quando aplicável. Informe o destinatário na operação com entrega.
- Use `TpEmis::Contingencia_Offline` (`tpEmis=9`) para assinar o XML e o QR
  Code em contingência. O XML assinado pode ser reimportado e enviado depois
  pelo serviço de autorização da NF-e modelo 55.

#### Distribuição de DF-e — NT 2014.002 v1.40

- Consulte o Ambiente Nacional por último NSU, NSU específico ou chave de
  acesso em `EventoNotaFiscal::nfeDistribuicaoDFe`, com CPF ou CNPJ
  do interessado, inclusive alfanumérico.
- Leia o lote em `retDistDFeInt->loteDistDFeInt->docZip`. Cada item
  expõe o Base64 recebido em `get_zip()` e o XML descompactado em
  `get_xml()`, com acesso aos resumos, ao evento processado e à NF-e com
  protocolo.

#### Produtos agropecuários — NT 2024.003 v1.10

- Leia e gere o grupo opcional `agropecuario` da NF-e e da NFC-e em
  `InfNFe::agropecuario`. Preencha de 1 a 20 receituários em `defensivo`,
  confirmando cada registro com `add()`, ou preencha uma única
  `guiaTransito`, com série opcional.
- A leitura e a geração preservam os zeros à esquerda no CPF do responsável
  técnico e no número da guia. Recompile a biblioteca e as aplicações devido
  à alteração na classe `InfNFe`.

#### Manifestação do destinatário — NT 2020.001 v1.60

- Envie Confirmação da Operação (210200), Ciência da Operação (210210),
  Desconhecimento da Operação (210220) e Operação não Realizada (210240) por
  `EventoNotaFiscal::envEvento`. Informe a chave da NF-e modelo 55, a data
  do evento e o CPF ou CNPJ do destinatário, com o certificado correspondente.
  O lote aceita até 20 manifestações e segue para o Ambiente Nacional.
- Informe `nSeqEvento=1` na Ciência e `nSeqEvento=1` ou `2` nas manifestações
  conclusivas. Em Operação não Realizada, preencha `detEvento->set_xJust()`
  com 15 a 255 caracteres. O XML inclui a justificativa somente nesse tipo.
- O XML processado fica disponível para os retornos 135 e 136. A SEFAZ
  verifica o prazo de 10 dias para Ciência e o prazo de 90 dias para a
  manifestação conclusiva, contados da autorização da NF-e.

#### Diferimento e partilha do ICMS — NT 2022.002 v1.30a

- O CST 90 aceita `cBenefRBC` e os grupos de diferimento de ICMS
  (`vICMSOp`, `pDif` e `vICMSDif`) e de FCP (`pFCPDif`, `vFCPDif` e
  `vFCPEfet` opcional), com leitura e geração de valores iguais a zero
  quando informados.
- Use `CstICMS::ICMSPart20` para informar o CST 20 no grupo `ICMSPart`.
  O grupo de partilha aceita `vICMSDeson`, `motDesICMS` e
  `indDeduzDeson`, incluindo os motivos 10 e 11 para venda de veículos
  destinados a pessoas com deficiência. Esses motivos também estão
  disponíveis no grupo `ICMS20`. Recompile a biblioteca e as aplicações
  devido à alteração na classe `Icms`.

#### Emitente CPF na NFC-e — NT 2023.002 v1.01

- Para emitir NFC-e de produtor rural em aplicativo próprio, informe o CPF
  com 11 dígitos, a inscrição estadual vinculada, a série de 920 a 969 e
  configure o e-CPF do emitente. A biblioteca valida o CPF e completa o
  documento com zeros à esquerda na chave de acesso.
- A geração aceita o processo de emissão pelo Fisco (`procEmi=1` ou `2`)
  na NFC-e de Santa Catarina, inclusive em contingência off-line (`tpEmis=9`).

#### NFC-e e NF-e — NT 2025.001 v1.03

- Selecione `VersaoQrCodeNF::v300` para emitir NFC-e com QR Code versão 3
  sem CSC. Na contingência, a biblioteca assina os parâmetros do QR Code com
  o mesmo certificado digital usado no XML.
- A NFC-e com emitente identificado por CPF usa automaticamente o QR Code
  versão 3, exceto no Paraná, onde segue a configuração. A versão 2 continua
  disponível para os demais emitentes. O destinatário da NFC-e pode ser
  identificado por documento estrangeiro.
- `TPag::PagamentoPosterior` (`tPag=91`) está disponível na leitura e na
  geração de NF-e e NFC-e. Informe `vPag=0.00` para essa forma de pagamento.

#### Benefícios fiscais do ICMS — NT 2019.001 v1.70

- Leia e grave até quatro créditos presumidos por item em `Prod::gCred`.
  Preencha `cCredPresumido`, `pCredPresumido` e `vCredPresumido` no objeto do
  contêiner e confirme cada grupo com `add()`.
- Use `Icms::set_cBenefRBC()` para informar o código do benefício de redução
  de base de cálculo acumulado com diferimento no CST 51. A aplicação
  fornece os códigos e valores de acordo com a tabela e as regras da UF.
  Recompile a biblioteca e as aplicações devido às mudanças em `Prod` e `Icms`.

#### Documentação

- Logotipo do C++ Brasil no topo do README.

#### NF-e e NFC-e — NT 2018.005 v1.52

- `InfRespTec::set_CSRT()` permite informar o código do responsável técnico
  por nota. Informe também `idCSRT` e os dados de contato. Na geração do XML,
  a biblioteca usa a chave de acesso da nota para calcular `hashCSRT` com
  SHA-1 e Base64. Use `set_hashCSRT()` para fornecer um hash pronto.
- `Retirada` e `Entrega` aceitam números de endereço como `S/N` e `12A`
  por `set_nroTexto()` e `get_nroTexto()`. O acesso inteiro continua disponível.
- `ProtNFe::get_mensagens()` expõe todas as mensagens da SEFAZ em ordem e
  conserva os zeros à esquerda do código. `get_cMsg()` e `get_xMsg()` dão
  acesso à última mensagem.
- Recompile a biblioteca e as aplicações que a utilizam devido às alterações
  no layout das classes.

#### Combustíveis monofásicos — NT 2023.001 v1.60

- NT 2023.001 v1.60: `Comb::pBio`, com leitura, escrita e limpeza, independente
  do grupo de encerrante. Mantido o acesso legado por `Encerrante::pBio`, agora
  inicializado e limpo corretamente; quando ambos forem preenchidos, o valor
  definido por `Comb::set_pBio()` prevalece, inclusive zero para omitir a tag.
- Leitura de `origComb`, dos campos monofásicos dos CSTs 02/15/53/61 e dos seis
  totais correspondentes. Preservados os percentuais de redução, seus motivos,
  as quantidades com até quatro casas nos itens e os totais com duas casas.
- Consultas `Icms::has_qBCMono()`, `has_qBCMonoReten()`, `has_qBCMonoRet()`,
  `has_adRemICMS()`, `has_vICMSMono()`, `has_vICMSMonoOp()`, `has_pDif()`,
  `has_vICMSMonoDif()` e `has_pRedAdRem()` para distinguir campos opcionais
  ausentes de valores explicitamente informados, inclusive zero.

##### Orientações de uso

Use os grupos existentes em `det->imposto->ICMS`, com `CstICMS::ICMS02`,
`ICMS15`, `ICMS53` ou `ICMS61`. A biblioteca conserva os valores informados;
não calcula o imposto, os saldos ou os totais automaticamente.

Informe o índice de mistura em `det->prod->comb->set_pBio(...)`: percentual
de biodiesel no Diesel B ou de etanol anidro na Gasolina C, maior que zero e
até 100, com até quatro casas. Zero omite a tag. O campo é filho de `comb`
e pode ser utilizado sem dados de encerrante.

Preencha cada origem em `comb->origComb->obj`, com `set_indImport(...)`,
`set_cUFOrig(...)` e `set_pOrig(...)`, e confirme com `origComb->add()`.
O leiaute permite até 30 origens. A exigência ou proibição de `pBio` e de
`origComb` depende da tabela ANP e da operação, incluindo as exceções para
consumidor final, complementar/devolução e CFOP 5922/6922. A v1.60 ativa
LA18-10, que exige origem conforme a tabela e suas exceções.

Nos CSTs 02/15/53, use `qBCMono`, `adRemICMS` e `vICMSMono`. No CST 15,
preencha também `qBCMonoReten`, `adRemICMSReten` e `vICMSMonoReten`;
se houver redução, informe juntos `pRedAdRem` e `motRedAdRem` (1 ou 9).
No CST 53, os campos opcionais `vICMSMonoOp`, `pDif`, `vICMSMonoDif`
e `vICMSMono` representam operação, percentual diferido, valor diferido
e valor devido. Para diferimento total, informe `pDif=100` e os valores
correspondentes, inclusive zero quando aplicável. No CST 61, use
`qBCMonoRet`, `adRemICMSRet` e `vICMSMonoRet`.

A v1.60 amplia a lista ANP especial de N41-10/N41-20 para incluir Diesel B.
Na hipótese descrita para refinarias ou suas bases, a retenção do CST 15 usa
`qBCMonoReten * adRemICMSReten * 0,3333`, com arredondamento a duas casas;
o fator não se aplica indistintamente a todos os combustíveis. O exemplo
da NT informa quantidade 140, alíquota 1,12 e retenção 52,26. São valores
ilustrativos; utilize a alíquota e a mistura aplicáveis à operação.

Preencha os seis totais monofásicos em `total->ICMSTot` com os somatórios
correspondentes, a duas casas. Observe a inclusão de `vICMSMonoReten` em
`vNF` conforme W16-10 e as demais regras vigentes de composição do total.
Informe os textos exigidos pela seção 6 em `infAdic->infCpl` e em
`det->infAdProd`; a biblioteca não os compõe automaticamente. A apresentação
das informações adicionais por produto depende do componente de DANFE utilizado.

`validar()` verifica a estrutura pelo XSD. Tabela ANP, unidade tributável,
exceções, cálculos, tolerâncias e demais regras fiscais dependem dos dados
preparados pela aplicação e das validações da SEFAZ. Utilize schemas atualizados.

#### Prorrogação da suspensão do ICMS — NT 2015.001 v1.30

- NT 2015.001 v1.30: pedidos de prorrogação da suspensão do ICMS para o primeiro
  e segundo prazos (`111500`/`111501`) e seus cancelamentos (`111502`/`111503`)
  em `EventoNotaFiscal::envEvento`, exclusivos da NF-e modelo 55. Versão 1.00,
  descrição, órgão da UF e ID são preparados pela biblioteca; informar CNPJ/CPF
  do autor, chave, data do evento e sequência de 1 a 99, controlada pela aplicação.
  Enviar em lote separado de outras famílias, com até 20 eventos e UF/ambiente
  correspondentes à configuração. Recompilar a biblioteca e as aplicações pela
  ampliação de `DetEvento`.
- Nos pedidos, preencher `detEvento->nProt` com o protocolo de autorização da NF-e
  e adicionar de 1 a 990 elementos em `detEvento->itemPedido`, com `numItem`
  único de 1 a 990 e `qtdeItem` decimal, gravada com quatro casas. Nos cancelamentos,
  preencher `set_idPedidoCancelado()` com o ID completo do pedido do mesmo prazo
  e da mesma NF-e (`ID` + tipo + chave + sequência de dois dígitos), e `set_nProt()`
  com o protocolo de registro desse pedido. Cancelamentos não contêm itens.
- Validação do envelope e do detalhe pelos XSDs `e111500_v1.00.xsd` a
  `e111503_v1.00.xsd`. O registro com `cStat=135` gera `procEventoNFe`, associado
  por chave, tipo, sequência e ambiente. `enviarEvento()` retornar `true` não
  significa deferimento: consultar os códigos do lote e do evento, e acompanhar
  a decisão posterior do Fisco.
- Leitura das respostas do Fisco `411500`–`411503`, com os tipos
  `FiscoPedidoProrrogacao1`, `FiscoPedidoProrrogacao2`,
  `FiscoCancelPedidoProrrogacao1` e `FiscoCancelPedidoProrrogacao2`.
  `DetEvento::idPedido` identifica a solicitação original;
  `respPedido` contém `statPrazo` e `itemPedido` com `numItem`, `statPedido`,
  `justStatus` e `justStaOutra`; `respCancPedido` contém `statCancPedido`,
  `justStatus` e `justStaOutra`. `statPrazo=0` significa após o prazo e `1` dentro
  do prazo; o valor inicial `-1` indica ausência. Decisões `1`/`2` significam
  deferido/indeferido. Respostas sucessivas devem ser relacionadas por `idPedido`
  e pelos itens; o sequencial do Fisco é independente do sequencial do pedido.
- `TEvento::lerXMLProrrogacao(xml, configuracoes)` importa um `evento` ou
  `procEventoNFe` da NT, validando envelope, detalhe específico e vínculo do pedido.
  Limpa os dados anteriores, inclusive em caso de erro, e preserva o documento
  recebido em `get_XMLOriginal()`. Exemplo com XML obtido pela aplicação:

  ```cpp
  TEvento resposta;
  if (resposta.lerXMLProrrogacao(xmlRecebido, cppnfe->configuracoes.get())) {
      const auto &det = *resposta.infEvento->detEvento;
      QString pedido = det.get_idPedido();
      // Consultar respPedido ou respCancPedido conforme o tipo do evento.
  }
  ```

  A importação não consulta `NFeDistribuicaoDFe` nem verifica criptograficamente
  a assinatura recebida. A aplicação deve obter o XML e verificar sua autenticidade.
  A geração e o envio de eventos exclusivos do Fisco são bloqueados. A consulta de
  protocolo também lê os novos grupos quando presentes no retorno.
- Orientações da v1.30: a solicitação completa adotada por MG exige todos os itens
  e quantidades da NF-e, inclusive no segundo prazo; a solicitação parcial de SP
  permite selecionar itens e quantidades. A aplicação deve preparar o pedido
  conforme a UF e acompanhar os saldos, prazos e decisões. Os limites de pedidos
  pendentes são definidos pela UF, e `justStatus=7` não se aplica à solicitação
  completa. O cancelamento do primeiro prazo pode depender do cancelamento prévio
  do segundo; pedidos vigentes podem impedir o cancelamento da NF-e (rejeição 811).
  Essas condições dependem do estado fiscal mantido pela SEFAZ.

#### Cancelamento de NF-e e NFC-e

- `CancNFe`, em `cancnfe.h`/`cancnfe.cpp`, disponível em `EventoNotaFiscal::cancNFe`,
  é o caminho do cancelamento de NF-e/NFC-e (`TpEvento::Cancelamento`, 110111, padrão) e do
  cancelamento por substituição exclusivo da NFC-e (`TpEvento::CancelamentoSubstituicao`,
  110112), conforme o leiaute 1.00. Usa a configuração e o certificado definidos em
  `cppnfe->configuracoes`. Recompilar a biblioteca e as aplicações devido à ampliação
  de `EventoNotaFiscal`.
- Os campos do evento ficam na própria `CancNFe`, sem `TEvento`: preencher CNPJ **ou** CPF
  do emitente, chave, protocolo de autorização e justificativa de 15 a 255 caracteres.
  Versão, descrição, ID, sequência 1, ambiente e órgão são preparados pela biblioteca, e a
  data não informada usa o horário do envio. Informar modelo e UF correspondentes à nota.
  Exemplo, com os schemas configurados e o certificado do emitente carregado em
  `cppnfe->configuracoes`:

  ```cpp
  cppnfe->eventos->cancNFe->clear();
  cppnfe->eventos->cancNFe->set_CNPJ(cnpjEmitente); // ou set_CPF(cpfEmitente)
  cppnfe->eventos->cancNFe->set_chNFe(chaveNFe);
  cppnfe->eventos->cancNFe->set_nProt(protocoloAutorizacao);
  cppnfe->eventos->cancNFe->set_xJust(justificativa);
  bool enviado = cppnfe->eventos->cancNFe->cancelar(1); // idLote
  // A decisão da SEFAZ está em cppnfe->eventos->cancNFe->retorno.
  ```

  Para substituição, definir também `set_tpEvento(TpEvento::CancelamentoSubstituicao)`,
  `set_chNFeRef(chaveSubstituta)` e `set_verAplic(versaoAplicativo)`. A nota cancelada deve
  ter emissão normal e a substituta deve ser distinta, com dígito verificador válido, da
  mesma UF, emitente e modelo, emitida em contingência, no mesmo ano e mês da nota
  cancelada ou no mês anterior. Usar `clear()` antes de preparar outro pedido.
- O autor informado é conferido com o CNPJ/CPF da chave antes do envio, considerando a
  série da nota.
- `RetEventoCancNFe`, em `CancNFe::retorno`, reúne o resultado do lote e do evento.
  `get_cStat()`, `get_xMotivo()`, `get_nProt()` e `get_dhRegEvento()` trazem a decisão sobre
  o evento; `get_cStatLote()`, `get_xMotivoLote()` e `get_XMLLote()` trazem os dados do lote.
  Se a SEFAZ recusar o lote, `get_cStat()` permanece 0 e `get_XML()` fica vazio; consultar
  os dados do lote. `get_XML()` traz o `procEventoNFe` quando homologado e, nos demais
  casos, o `retEvento` recebido.
- Assinatura do cancelamento e validação pelos XSDs específicos `envEventoCancNFe` /
  `envEventoCancSubst`, `retEnvEventoCancNFe` / `retEnvEventoCancSubst` e
  `procEventoCancNFe` / `procEventoCancSubst`, todos na versão 1.00.
  O XML `procEventoNFe` é montado e validado para lote 128 com evento 135 ou 155,
  somente quando chave, tipo, sequência e ambiente correspondem ao pedido; sua gravação
  segue a configuração de arquivos de eventos. `cancelar()` retornar `true` indica que o
  envio e o tratamento da resposta concluíram: a aceitação fiscal depende dos códigos
  retornados. Prazos, autorização, protocolo e impedimentos fiscais dependem da SEFAZ.
- `EnvEvento::enviarEvento()` passa a recusar os eventos 110111 e 110112, que devem ser
  enviados por `cancNFe`. A consulta de protocolo continua a devolver os eventos da nota,
  inclusive cancelamentos, em `procEventoNFe`.

#### Informações de pagamentos e outros — NT 2023.004 v1.20

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

##### Orientações de uso

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

#### MEI — CRT 4 — NT 2024.001 v1.20

- Suporte a `Crt::SimplesMEI` (CRT 4) da NT 2024.001 v1.20, incluindo conversão
  na leitura e geração. Nos grupos `ICMSSN102` e `ICMSSN900`, a origem não preenchida
  (`Orig::None`) é omitida para MEI; origens informadas, inclusive nacional (0), são preservadas.

##### Orientações de uso

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

#### Evento ECONF — NT 2024.002 v1.00

- Evento de Conciliação Financeira da NT 2024.002 v1.00, usando diretamente
  `EventoNotaFiscal::envEvento`, `TEvento` e `RetEnvEvento`. Inclui os eventos
  `TpEvento::ECONF` (110750) e `TpEvento::CancelECONF` (110751), os pagamentos em
  `DetEvento::detPag` (`Container<DetPag>`), geração e assinatura XML, schemas específicos
  de envio/retorno e direcionamento da NF-e para a SVRS e da NFC-e para a UF.
- `DetPag` ampliada com `dPag`, `CNPJPag`, `UFPag`, `CNPJReceb` e `UFReceb`,
  reutilizando `Card` para os dados da instituição de pagamento. Aplicações devem ser
  recompiladas devido à alteração do layout da classe.

##### Orientações de uso

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
de dependência.
Envie conciliação e cancelamento em lotes separados, com até 20 eventos; cada ECONF aceita
de 1 a 100 pagamentos. A biblioteca preenche a descrição, versão do detalhe e órgão receptor:
NF-e usa SVRS (92); NFC-e usa o serviço da UF. No XSD fornecido, `CNPJIF`, `tBand` e `cAut`
dependem do preenchimento de `CNPJPag` e `UFPag`.

#### API de containers

- `Container::count()`, `isEmpty()`, `value(i)` (retorna `nullptr` para índice inválido) e `begin()`/`end()`,
  que permitem escrever `for (auto *x : *container)`.

### Modificado

#### Autorização com alerta — NT 2026.002 v1.11

- O status `120` é tratado como autorização na emissão e na consulta de
  protocolo. O XML processado preserva a sequência de até cinco mensagens
  da SEFAZ, disponível em `ProtNFe::get_mensagens()`.

#### Acesso aos dados da NF-e

- Use os métodos `get_codigo()`, `set_codigo()`, `get_texto()` e
  `set_texto()` de `ProtNFe::MensagemSefaz` para acessar os campos de cada
  mensagem do protocolo. `GuiaTransito::set_tpGuia()` passa a receber
  `const int &`. Recompile a biblioteca e as aplicações.

#### Autorização de NFC-e — NT 2023.002 v1.01

- O envio recusa lotes que contenham uma NFC-e e outro documento antes de
  chamar o serviço de autorização. Envie cada NFC-e em um lote síncrono.

#### Autorização — NT 2025.001 v1.03

- Lotes com uma única NF-e ou NFC-e solicitam resposta síncrona
  automaticamente e tratam o retorno sem consultar recibo.

#### Estrutura do projeto

**Alteração incompatível:** quem usa a biblioteca precisa ajustar includes e links.

- Os módulos passam a ficar na raiz do projeto, cada um com sua biblioteca:
  `CppBase` (arquivos comuns), `CppNFe`, `CppDanfe` (base do DANFE, código de
  barras e QR Code) e `DanfeQtRPT`. Os arquivos comuns deixam de depender da
  NF-e e do DANFE, e a CppNFe segue sem exigir Qt Gui nem Qt Widgets.
- Nova biblioteca `libCppBase`, que deve ser ligada junto com a `libCppNFe`. Para o
  DANFE, ligue também a `libCppDanfe`, além da `libCppDanfeQtRPT`.
- Os includes `<CppBrasil/NFe/...>`, `<CppBrasil/DanfeQtRPT/...>` e
  `<CppBrasil/...>` passam a ser `<CppNFe/...>`, `<DanfeQtRPT/...>`, `<CppBase/...>`
  ou `<CppDanfe/...>`, este último para `cppdanfe.h`, `code128.h`, `cppbarcode.h`
  e `qrcodegen.hpp`. Os headers são instalados nas mesmas pastas, em `include/`.
- Um único `CMakeLists.txt` na raiz compila todos os módulos. Use
  `-DCPPBRASIL_BUILD_DANFE=OFF` para compilar sem o DANFE e sem o QtRPT, ou informe
  `-DQTRPT_INCLUDE_DIR` e `-DQTRPT_LIBRARY`. Os scripts de instalação usam esse
  arquivo e pedem o caminho das dependências.
- As macros de exportação dos arquivos comuns passam de `CPPNFE_EXPORT` para
  `CPPBASE_EXPORT`, e `CppDanfeQtRPT` passa a usar `CPPDANFEQTRPT_EXPORT`.
- Símbolos sem macro de exportação ficam ocultos também no Linux e no macOS, como
  já acontecia no Windows.

As alterações de compatibilidade abaixo exigem atenção ao atualizar a biblioteca e as aplicações.

#### NT 2023.001: recompilar as aplicações

As classes `Comb` e `Icms` receberam membros para o índice de mistura e o controle
de presença de campos opcionais. Recompile a biblioteca e as aplicações que a utilizam.
Nos CSTs monofásicos, quantidades opcionais não preenchidas passam a ser omitidas;
chamar o setter com zero mantém a tag com zero. No CST 53, o mesmo vale para
alíquota, valores e percentual de diferimento. `clear()` restaura a ausência.

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

#### Validação XML e gerenciamento de recursos

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
- O layout interno de `CppDanfeQtRPT` mudou: as aplicações que usam a lib do DANFE precisam ser recompiladas.

### Obsoleto

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

### Removido

- Dependências pré-compiladas para Linux, Windows e Android. Elas devem ser instaladas
  à parte. O código-fonte do QtRPT passa a acompanhar o módulo `DanfeQtRPT`.
- Projetos qmake (`.pro`). O CMake é o único sistema de build.

- Dependências pré-compiladas antigas para Windows: OpenSSL 3.1.3, libxml2 2.11.5,
  zlib 1.3 e libiconv. As versões novas serão distribuídas como pacotes compactados.
  Até lá, o instalador MSVC e os exemplos precisam de ajustes nas referências às dependências.
- Exemplo legado QtCon-2022, sem manutenção.
- Suporte ao Qt 5, cujo suporte oficial terminou em 2025. A biblioteca e os
  exemplos passam a exigir o Qt 6.5 ou superior.

### Corrigido

#### Ordem do ISSQN no XML da NF-e

- O grupo `ISSQN` do item passa a ser gerado antes do `PIS`, na ordem
  exigida pelo schema. Antes ele era gerado após o `ICMSUFDest`, o que
  tornava inválida a nota com serviço tributado pelo ISSQN.

#### Descarregamento do certificado digital

- O descarregamento limpa os ponteiros das estruturas do OpenSSL para
  evitar liberação duplicada ao descarregar novamente ou destruir o objeto.
  A inutilização recusa a geração com certificado ausente ou descarregado.

#### EPEC da NF-e — NT 2014.001 v1.41

- Gere o evento 110140 pelo `EventoNotaFiscal::envEvento`, em lote
  exclusivo para o Ambiente Nacional. Use a chave da NF-e modelo 55 com
  `tpEmis=4` e informe CPF ou CNPJ do emitente.
- O detalhe grava a sigla da UF do destinatário a partir do código IBGE,
  com `99` para `EX`. Para destinatário no exterior, a identificação
  estrangeira pode ser vazia. A IE do destinatário é omitida para isentos
  e operações com o exterior. Os totais são gravados após o grupo `dest`.

#### Chave de acesso com CNPJ alfanumérico — NT 2026.004 v1.01

- A geração da chave da NF-e e da NFC-e aceita letras nas 12 primeiras
  posições do CNPJ e calcula o dígito verificador com o valor ASCII menos 48.
- As validações de cancelamento, substituição, manifestação e prorrogação
  aceitam chaves alfanuméricas. A validação XML dos eventos requer schemas
  compatíveis com os respectivos identificadores.
- A validação local com certificado digital cobriu a assinatura e a
  reimportação de NF-e e NFC-e com emitente alfanumérico, o QR Code v3
  on-line e em contingência e a assinatura dos eventos testados.
- O schema genérico de eventos do PL 010d v1.03 validou os envelopes
  assinados de cancelamento, cancelamento por substituição e cancelamento
  de prorrogação com chave alfanumérica.

#### Destinatário, transporte e referência da NF-e — NT 2022.002 v1.30a

- A NF-e grava um único identificador do destinatário. Em operações com o
  exterior sem CNPJ ou CPF, grava `idEstrangeiro` mesmo quando o conteúdo
  está vazio. Nos casos de equiparação à exportação, a biblioteca conserva
  o documento, o endereço e a indicação de IE fornecidos pela aplicação.
- O grupo `transporta` é gravado na hierarquia correta quando informado.
  Na operação presencial com CFOP 7667 e `UFCons=EX`, use `modFrete=9`
  para indicar ausência de frete e omitir o transportador.
- A importação preserva `refNFeSig` para a regeneração do XML, inclusive
  em NF-e de ajuste (`finNFe=3`). Informe a chave já com o código numérico
  zerado por `NFRef::set_refNFeSig()`.

#### Cancelamento — NT 2023.002 v1.01

- O cancelamento confere o dígito verificador da chave e compara o documento
  do autor com o documento completo na chave. Para chaves avulsas da série
  890 a 899, aceita CPF ou CNPJ conforme o autor. Para chaves da NFF, identifica
  o tipo de documento pelo quinto dígito do número da nota.

- `OrigComb` não era exportada pela biblioteca, o que impedia o uso direto da classe
  por aplicações no Windows.

#### NF-e e NFC-e

- O XML grava `cNF` com oito dígitos, incluindo os zeros à esquerda usados na
  chave de acesso. Os campos opcionais do diferimento no CST 51 preservam
  zeros informados, inclusive `vICMS` no diferimento total.
- NT 2018.005 v1.52. `idCSRT` é gravado com dois dígitos e importado no campo
  correto. Os locais de retirada e entrega preservam o CEP com oito dígitos
  e podem ser informados com CNPJ vazio quando o documento não estiver disponível.
- O repasse de ICMS-ST grava os valores da UF de destino independentemente do
  FCP. O XML preserva valores zero informados no ICMS-ST retido, FCP retido e
  ICMS efetivo. A geração automática dos campos da rejeição 938 fica restrita
  ao modelo 55.
- A importação de NF-e processada recupera as mensagens da SEFAZ e o atributo
  `Id` de `infProt`. A montagem do protocolo conserva as mensagens lidas e
  omite `nProt` e `digVal` quando estiverem ausentes.
- Leitura dos combustíveis: `pGNi`, `nBomba` e `vEncFin` passam a preencher seus
  próprios campos, sem sobrescrever `pGNn`, `nBico` e `vEncIni`, respectivamente.
- CST 15: `pRedAdRem=0` explicitamente informado é preservado na geração.
  A sequência de redução exige também `motRedAdRem`; pares incompletos são
  rejeitados pela validação por XSD.
- Conversão de leitura dos CSTs do ICMS: agora usa a posição do enum, em vez do
  número fiscal, preservando também Part10/Part90/ST41/ST60. Antes a leitura podia
  resultar em CST incorreto ou acesso inválido ao gerar novamente.
- CST 41/50 passam a gerar o grupo compartilhado `ICMS40`, preservando o CST;
  ICMSPart10/90 usam `ICMSPart` (a identificação do grupo agora precede o corte do CST).
- Ato concessório 08 passa a ser escrito com dois dígitos; leitura aceita 08 e 8.
- CSOSN 103, 300 e 400 passam a usar o grupo XML `ICMSSN102`, conservando o valor de
  `CSOSN`. Antes eram gerados grupos inexistentes (`ICMSSN103`, `ICMSSN300` e `ICMSSN400`).
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
- Valores padrão em `ConfigNFe`: `ModeloDF::NFe`, `VersaoNF::v400`, `VersaoQrCodeNF::v200`, `IndSinc::Nao` e `TpEmis::Normal`.

#### `gerar()` valida a chave de acesso

Se a chave de acesso montada (sem o dígito verificador) não tiver exatamente 43 dígitos numéricos, `gerar()`
passa a devolver `false` com a mensagem `Chave de acesso invalida (...)`. Antes, a nota era gerada em silêncio
com uma chave que a SEFAZ rejeitaria. A causa mais comum é o **CNPJ/CPF do emitente com máscara**
(`12.345.678/0001-90`): informe somente os números.

#### Eventos

- Preservação do atributo `infEvento/@Id` na leitura do retorno de `WSEvento`.
- `InfEvento::set_CPF()` gravava no membro CNPJ, impossibilitando informar corretamente
  o CPF do autor nos eventos.
- A associação de retorno e evento passa a conferir também `tpEvento`, além da chave e sequência.
- `EnvEvento::enviarEvento()` passa a retornar a falha do web service corretamente; a geração
  repetida limpa o XML do lote e o erro/assinatura anterior do evento.

#### Serviços e consultas públicas

- Endereços dos serviços NF-e/NFC-e e consultas públicas atualizados.
  Corrigidos o uso de produção na homologação da
  NFC-e de SP, os serviços de GO apontando para CE, o retorno de autorização de GO,
  o roteamento da NFC-e de CE para SVRS e os endereços de cadastro e contingência.
  Atualizadas também as URLs de QR Code/consulta, removidos espaços externos e
  corrigida a base de SC para evitar duplicação de `?p=`. Recompilar a biblioteca
  para incorporar os novos endereços ao recurso embarcado.
- **`HttpOpenSSL`** (usado com `HttpType::HttpOpenSSL`; o padrão `HttpQt` não é afetado):

  - a resposta vinha com lixo de memória depois do XML, e respostas grandes podiam vir cortadas. Agora a leitura
    respeita o `Content-Length`, decodifica `Transfer-Encoding: chunked` e aceita servidor que fecha sem `close_notify`;
  - não tinha timeout e podia travar a aplicação. Agora usa o timeout configurado (padrão de 15 s, como o `HttpQt`);
  - XML com acentos podia ser corrompido no Windows (conversão para a codificação local) e o `Content-Length` ficava errado;
  - linha do pedido HTTP inválida (espaço antes do fim de linha), SNI não enviado e `SSL_accept` indevido no cliente;
  - socket, `SSL_CTX` e `SSL` vazavam nos retornos de erro.
- `HttpQt`: o cabeçalho `Content-Type` era enviado com erro de digitação (`charst=utf-8`), e o charset era ignorado.
- Falha ao abrir o `urlnfe.json` interno agora gera um aviso claro, em vez de um erro enganoso de parse do JSON.

#### DANFE e chave de acesso

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

#### XML e assinatura

- A assinatura alterava configurações globais da libxml2, afetando todos os parses seguintes (validação por XSD e
  retornos da SEFAZ). As opções agora valem só para a assinatura. A assinatura gerada não muda.
- **Assinatura no Fedora/RHEL** (e derivados): falhava sempre com "Erro ao assinar documento", porque a lib ignorava
  a configuração do OpenSSL e o SHA1 ficava bloqueado sem como liberar. Agora basta liberar o SHA1 (ver README).
- **Crash na assinatura** (`CppLibXml2::c14n`): quando o nó a assinar não existia no XML, a lib liberava a raiz do documento
  duas vezes (`double free`) e a aplicação fechava. Agora a assinatura falha com erro.
- A assinatura chamava `xmlCleanupParser()`, que limpa o estado global da libxml2 e não deve ser chamada com a biblioteca em uso
  (risco com threads ou com outras partes da aplicação que usam a libxml2).
- Validação por XSD (`parseXML`): os erros da libxml2 eram impressos no stderr da aplicação e a mensagem de erro terminava com
  quebra de linha. Agora os erros são capturados pela lib, e a mensagem informa o **primeiro** erro do XML (antes era o último).
  Diretório de schemas inválido encerra a validação na hora.
- `CppXML::setNode` (`CppLibXml2`): valor com `&` ou `<` era tratado como XML já escapado. Na libxml2 2.12 a tag ficava
  vazia (e o erro saía no stderr); na 2.15 o `&` era descartado. Agora o valor é gravado como texto, escapado corretamente,
  igual nas duas versões. A assinatura não é afetada.

#### Containers e exemplos

- `Container::add()` ficou seguro contra exceção.
- Exemplo `consoleNFe`: usava `StatusServico::get_status()`, renomeado para `status()`.

#### Ator interessado na NF-e — Transportador — NT 2020.007 v1.40

- Evento Ator Interessado na NF-e — Transportador (110150), conforme NT 2020.007 v1.40:
  reutiliza `EnvEvento`, `TEvento`, `DetEvento::autXML` e os enums existentes. Corrigida a tag
  `tpAutorizacao` e a emissão dos opcionais; valida as combinações de autor/autorização e
  restringe o evento à NF-e modelo 55. Preenche descrição, versão 1.00, órgão 91 e condição
  de uso quando autorizada. Direciona ao Ambiente Nacional em lote separado, usa os XSDs
  específicos de envio/retorno e gera XML processado para os status 135 e 136.

##### Orientações de uso

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
`procEventoAtorInteressado_v1.00.xsd`.
A versão 1.40 da NT mantém o leiaute 1.00; a regra 4P24-10, sobre habilitação do autorizado
como emitente de CT-e, depende do cadastro consultado pela SEFAZ. O serviço também verifica
o prazo de seis meses e os demais requisitos fiscais do evento.

### Segurança

- **Assinatura com DOCTYPE** (`signXML`): um XML com `<!DOCTYPE>` e entidade externa fazia o conteúdo de arquivos locais
  entrar no XML assinado (XXE), e podia acessar a rede. Agora XML com DOCTYPE é recusado na assinatura (o XML da NF-e
  não tem DOCTYPE; a assinatura das notas não muda).

[Não publicado]: https://github.com/cppbr/cppbrasil/commits/develop
