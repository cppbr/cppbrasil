# Pendências da libCppNFe

## NT 2025.002 v1.53 — leiaute RTC (IBS, CBS e IS)

- [x] Mapear no XML da NF-e/NFC-e os campos novos de `ide`, `emit` e `prod`,
  os grupos BB (`gCompraGov`), BC (`gPagAntecipado`), UB (`IS` e `IBSCBS`,
  com a monofasia da v1.50), VB (`vItem`), VC (`DFeReferenciado`) e os
  totais W31–W60, com leitura, geração e recusa de grupos exclusivos.
  Validado localmente no Windows com NF-e modelo 55: ida e volta idêntica
  e XSD do pacote local com assinatura apenas estrutural.
- [ ] Compilar a biblioteca completa e repetir a validação com libxml2 e
  assinatura real (Linux e Windows/MSVC), incluindo NFC-e modelo 65 com
  QR Code. O teste local substituiu o libxml2 e não assinou o XML.
- [ ] Homologar com a SEFAZ as regras do capítulo 7: CST/cClassTrib e
  indicadores `ind_g*`, totais W, finalidades 5 e 6 com `tpNFDebito`/
  `tpNFCredito`, `gCompraGov`, `cIndOp`, `vItem`/`DFeReferenciado` e o
  cronograma de obrigatoriedade.
- [ ] Avaliar a carga da tabela cClassTrib (Informe Técnico 2025.002) para
  validar, antes do envio, os grupos exigidos ou vedados pelos indicadores
  `ind_*`. Hoje a biblioteca gera o grupo preenchido e só recusa
  combinações exclusivas.
- [ ] Confirmar se algum caso fiscal exige grupo opcional com todos os
  valores zero (por exemplo `gIBSCBS` com `vBC=0`); hoje esses grupos não
  são emitidos.
- [ ] Conferir a procedência oficial do pacote de schemas da NT
  (`DFeTiposBasicos_v1.00.xsd` e leiaute da v1.50 ou posterior).
- [ ] Avaliar a seção 9 (DANFE) quando o componente entrar no escopo.

## Mapeamento de eventos ainda sem suporte — 08/11/2026

Levantamento documental e do código, sem implementação. Base principal:
MOC 7.0 – Visão Geral, tabelas 3-1 a 3-5 (p. 28–31), distribuição
(p. 90–98) e serviços de eventos (p. 99–123), em
`NT_PORTAL/Manual de Orientação ao Contribuinte - MOC - versão 7.0 - NF-e e NFC-e.pdf`.
Complemento: somente o capítulo 8 da NT 2025.002 v1.53 (p. 75–95),
disponível em `NT_PORTAL/`, e os detalhes de eventos nos schemas locais.
Não inclui revisão do leiaute da NF-e nem comprovação de vigência ou
disponibilidade dos serviços de cada UF. A presença de XSD não comprova
implementação nem procedência oficial do pacote.

### Critério de arquitetura e suporte já encontrado

- `EnvEvento` reúne `Container<TEvento>`, `InfEvento`, `DetEvento` e retorno.
  Eventos que usam a parte geral de recepção podem aproveitar esse fluxo,
  mesmo quando precisam de novos objetos individuais ou containers no detalhe.
  Ter schema específico ou cancelar outro evento não exige, por si só,
  uma nova classe de envio equivalente a `CancNFe`.
- `CancNFe` já atende 110111 (cancelamento da nota) e 110112 (substituição
  da NFC-e), com retorno próprio; `EnvEvento` recusa esses códigos.
  `InutNFe` já atende inutilização, que é serviço próprio e não um novo
  `tpEvento`. Status, consulta de protocolo e distribuição também já têm
  objetos próprios e não são eventos a implementar neste levantamento.
- Já há geração em `EnvEvento` para CC-e 110110; EPEC 110140 da NF-e;
  Ator Interessado 110150; manifestações 210200/210210/210220/210240;
  prorrogações 111500/111501 e cancelamentos 111502/111503;
  comprovante de entrega 110130 e cancelamento 110131;
  insucesso na entrega 110192 e cancelamento 110193;
  ECONF 110750 e cancelamento 110751. Não recriar essas implementações.
- As respostas fiscais 411500/411501/411502/411503 já têm modelagem e
  leitor `lerXMLProrrogacao`; conferir sua integração à distribuição
  conforme a pendência da NT 2014.002 já registrada abaixo.
- O importador de distribuição preserva o XML integral em `DocZip::get_xml()`
  e o código numérico em `ResEvento`, mas o evento processado usa
  `ConvNF::strToTpEvento`, que converte códigos não cadastrados em `None`.
  O leitor de `detEvento` da distribuição só preenche descrição, protocolo,
  justificativa e campos de CC-e. Isso não equivale à leitura especializada
  dos eventos listados a seguir.

### Eventos novos que podem usar EnvEvento — complemento RTC

Todos os 14 códigos de autoria do contribuinte abaixo estão ausentes de
`TpEvento`, das conversões e dos geradores específicos. Reaproveitar a
parte geral, adicionando os detalhes, validação específica, assinatura,
retorno e XML processado. A NT indica autorização pela SVRS e recomenda
um evento por requisição. Conferir o pacote oficial antes de implementar:
o texto ainda usa nomes de envelope com versão ilustrativa `9.99`.

| Pendente | Código | Evento | Objetos/dados específicos a mapear |
| --- | --- | --- | --- |
| [ ] | 112110 | Informação de efetivo pagamento integral para liberar crédito presumido do adquirente | Identificação do autor e indicação de quitação integral (seção 8.3). |
| [ ] | 112120 | Importação em ALC/ZFM não convertida em isenção | Itens, valores de IBS/CBS e controle de estoque (8.14). |
| [ ] | 112130 | Perecimento, perda, roubo ou furto durante o transporte contratado pelo fornecedor | Itens, valores, estoque e referências previstas no detalhe (8.16). |
| [ ] | 112140 | Fornecimento não realizado com pagamento antecipado | Itens, valores, estoque e documentos de antecipação conforme o detalhe (8.17). |
| [ ] | 112150 | Atualização da Data de Previsão de Entrega | Nova data de previsão (8.18). |
| [ ] | 211110 | Solicitação de Apropriação de crédito presumido | Itens e grupos de crédito presumido IBS/CBS, conforme o autor permitido (8.4). |
| [ ] | 211124 | Perecimento, perda, roubo ou furto durante o transporte contratado pelo adquirente | Itens, valores e controle de estoque (8.15). |
| [ ] | 211128 | Aceite de débito na apuração por emissão de nota de crédito | Identificação do autor e aceite (8.5). |
| [ ] | 211130 | Imobilização de Item | Itens, valores e quantidades imobilizadas (8.6). |
| [ ] | 211140 | Solicitação de Apropriação de Crédito de Combustível | Itens e grupo de consumo de combustível (8.7). |
| [ ] | 211150 | Solicitação de Apropriação de Crédito para bens e serviços que dependem de atividade do adquirente | Itens e grupos de crédito conforme a atividade (8.8). |
| [ ] | 212110 | Manifestação sobre Pedido de Transferência de Crédito de IBS em Operações de Sucessão | Manifestação da sucessora e valores previstos (8.9). |
| [ ] | 212120 | Manifestação sobre Pedido de Transferência de Crédito de CBS em Operações de Sucessão | Manifestação da sucessora e valores previstos (8.10). |
| [ ] | 110001 | Cancelamento de Evento | `tpEventoAut` e `nProtEvento`, autor e sequência do evento cancelado (8.13). Não é cancelamento de NF-e e não pertence ao `CancNFe`; limitar aos eventos admitidos pela NT. |

- [ ] Modelar os grupos repetidos dos eventos RTC com objetos e
  `Container<Classe>`, seguindo as ocorrências de cada detalhe. Definir
  esses objetos dentro do modelo de eventos; o levantamento não identificou
  necessidade de outra fachada de envio como `CancNFe` para esses códigos.
- [ ] Mapear a leitura de 412120 e 412130, manifestações do Fisco sobre
  transferência de crédito de IBS e CBS em sucessão (seções 8.11 e 8.12).
  Precisam de detalhes/objetos de leitura, identificação do Fisco e seu
  resultado; não disponibilizar emissão desses eventos ao contribuinte.

### Eventos do MOC sem leitura especializada — Fisco

Para todos os códigos desta seção, adicionar identificação/conversão e
mapear os detalhes conforme o BT correspondente. Reaproveitar os
envelopes de leitura/consulta/distribuição disponíveis, com objetos próprios
para detalhes quando necessário. Não enviar por `EnvEvento` e não criar
serviços de emissão fiscal para o contribuinte. Confirmar em cada BT quais
documentos completos ou resumos são efetivamente disponibilizados.

| Pendente | Código | Evento | Referência no MOC |
| --- | --- | --- | --- |
| [ ] | 400200 | Documento Fiscal Inidôneo | BT 2016.003 |
| [ ] | 400201 | Cancelamento do evento 400200 | BT 2016.003 |
| [ ] | 400300 | Visto Eletrônico do Fisco | BT 2018.002 |
| [ ] | 400301 | Cancelamento do evento 400300 | BT 2018.002 |
| [ ] | 400100 | Alerta: simulação de operação do emitente | BT 2016.003 |
| [ ] | 400101 | Cancelamento do evento 400100 | BT 2016.003 |
| [ ] | 400104 | Alerta: simulação de operação do emitente — Inex. | BT 2016.003 |
| [ ] | 400105 | Cancelamento do evento 400104 | BT 2016.003 |
| [ ] | 400120 | Alerta: mercadoria sem origem comprovada | BT 2016.003 |
| [ ] | 400121 | Cancelamento do evento 400120 | BT 2016.003 |
| [ ] | 500100 | Alerta: simulação de operação do destinatário | BT 2016.003 |
| [ ] | 500101 | Cancelamento do evento 500100 | BT 2016.003 |
| [ ] | 500104 | Alerta: simulação de operação do destinatário — Inex. | BT 2016.003 |
| [ ] | 500105 | Cancelamento do evento 500104 | BT 2016.003 |
| [ ] | 610500 | Registro de Passagem de NF-e | BT 2017.002 |
| [ ] | 610501 | Cancelamento do Registro de Passagem de NF-e | BT 2017.002 |

### Eventos do MOC sem leitura especializada — propagados e outros órgãos

Mesma orientação de leitura da seção anterior. Esses eventos são recebidos
de outros sistemas; seus objetos representam dados importados, sem
fachada de envio equivalente a `CancNFe`.

| Pendente | Código | Evento | Referência no MOC |
| --- | --- | --- | --- |
| [ ] | 790700 | Averbação de Exportação | BT 2017.001; detalhe local `e790700`. |
| [ ] | 410300 | NF-e Referenciada | BT 2013.004 |
| [ ] | 610510 | Registro de Passagem MDF-e | BT 2017.002 |
| [ ] | 610511 | Cancelamento do Registro de Passagem MDF-e | BT 2017.002 |
| [ ] | 610514 | Registro de Passagem MDF-e com CT-e | BT 2017.002 |
| [ ] | 610515 | Cancelamento do Registro de Passagem MDF-e com CT-e | BT 2017.002 |
| [ ] | 610552 | Registro de Passagem Automático MDF-e | BT 2017.002 |
| [ ] | 610554 | Registro de Passagem Automático MDF-e com CT-e | BT 2017.002; conferir no BT a descrição contraditória na tabela 3-4. |
| [ ] | 610600 | CT-e Autorizado | BT 2012.001 |
| [ ] | 610601 | CT-e Cancelado | BT 2012.001 |
| [ ] | 610610 | MDF-e Autorizado | BT 2013.007 / BT 2017.002 |
| [ ] | 610611 | MDF-e Cancelado | BT 2013.007 / BT 2017.002 |
| [ ] | 610614 | MDF-e Autorizado com CT-e | BT 2017.002 |
| [ ] | 610615 | Cancelamento do MDF-e Autorizado com CT-e | BT 2017.002 |
| [ ] | 990900 | Vistoria SUFRAMA | BT 2011.006; detalhe local `e990900`. |
| [ ] | 990910 | Internalização SUFRAMA | BT 2011.006; detalhe local `e990910`. |
| [ ] | 610130 | Comprovante de Entrega do CT-e propagado à NF-e | Distribuição, tabela da p. 91, nota 7; BT 2019.001 v1.10 e detalhe local `e610130`. |
| [ ] | 610131 | Cancelamento do Comprovante de Entrega do CT-e propagado à NF-e | Mesma referência; detalhe local `e610131`. |

### Coberturas parciais e pontos a confirmar antes de escolher a próxima tarefa

- [ ] EPEC da NFC-e, código 110140: o código atual exige modelo 55 e
  `tpEmis=4`; o MOC menciona EPEC para o modelo 65 e há detalhes locais
  separados `e110140NFe`/`e110140NFCe`. Mapear a NT específica da NFC-e,
  adoção por UF, campos, recepção e retorno. Avaliar extensão de `EnvEvento`
  ou objeto próprio somente após confirmar o contrato do serviço; não
  reutilizar automaticamente o EPEC da NF-e nem marcar modelo 65 como pronto.
- [ ] Comprovante de entrega 110130/110131: já há campos e geração, mas
  `TEvento::gerarXML` usa o mesmo bloco de campos opcionais nos dois tipos,
  e `EnvEvento::validarEvento` seleciona o envelope genérico. Revisar o
  detalhe próprio de cada tipo, validação e retorno segundo a NT 2019.001;
  manter em `EnvEvento`, salvo exigência documental posterior.
- [ ] Insucesso 110192/110193: geração e roteamento à SVRS já existem;
  revisar validação dos detalhes, retorno e importação conforme a NT
  2023.005. Não cadastrar novamente como eventos ausentes.
- [ ] Completar a importação dos detalhes de eventos já gerados pela
  biblioteca no fluxo `NFeDistribuicaoDFe`, reaproveitando o leitor específico
  de prorrogação quando aplicável. Manter o XML original e identificar os
  códigos recebidos mesmo quando o detalhe ainda não for reconhecido.
- [ ] Investigar 110300 — Vinculação da Transação de Pagamento no DF-e:
  há `e110300_v1.00.xsd` local, sem enum, objetos nem geração correspondentes.
  O detalhe contém `nProt` e `gPgto`/dados de transação e não é o ECONF
  110750. Localizar a especificação oficial, confirmar aplicabilidade à
  NF-e/NFC-e e vigência antes de promover a implementação; se confirmado
  o contrato de evento comum, a rota candidata é `EnvEvento` com objetos
  para o pagamento. A existência isolada desse XSD não basta para concluir.

Não abrir tarefa de emissão para Registro de Saída (descontinuado no MOC),
610550 (BRId, eliminado e substituído conforme BT 2017.002) nem 211120
(Destinação de item para consumo pessoal, eliminado no histórico da
NT 2025.002 v1.40, embora o XSD ainda esteja no pacote local). Suporte
histórico de leitura pode ser avaliado caso exista necessidade concreta.

Resumo do recorte: 34 códigos do MOC sem identificação/leitura especializada
(16 do Fisco e 18 propagados/outros órgãos, incluindo os dois comprovantes
do CT-e citados na distribuição); 16 códigos RTC ausentes (14 para envio
pelo contribuinte e dois do Fisco para leitura); um código adicional de
pagamento a confirmar e coberturas parciais descritas acima. Não foi
identificado novo evento desse recorte que comprovadamente exija outra
classe de envio como `CancNFe`; a avaliação do EPEC NFC-e permanece aberta.

## NT 2026.002 v1.11

- [x] Gerar, assinar, reimportar e validar o XML da NF-e modelo 55 com
  `tpImp=6` e QR Code v3, inclusive em contingência off-line. Exercitar
  o envio posterior e os retornos de autorização e consulta com alertas
  usando transporte simulado.
- [ ] Conferir o pacote oficial de schemas da NT para autorização e consulta,
  incluindo a ocorrência de até cinco mensagens e `cMsg` opcional.
  Os schemas de trabalho foram ajustados localmente para esses cenários.
- [ ] Homologar autorização, envio posterior em contingência e consulta com
  `cStat=120` e alerta 172, além de validar QR Code por UF e ambiente.
  Conferir os limites de valor, os grupos e as condições fiscais do Tipo 2
  e as regras I08-180, BA02-35 e VC02-40, previstas para 14/12/2026
  em produção. O modelo da chave referenciada pode ser lido localmente,
  mas identificar `tpImp=6` depende do XML da NF-e referenciada.

## NT 2026.003 v1.00 — Impressão do DANFE Simplificado Tipo 2

- [ ] Implementar o leiaute de impressão em `CppDanfe` e `DanfeQtRPT`
  para NF-e modelo 55 com `tpImp=6`, modalidade definida pela
  NT 2026.002 v1.11. Conferir os campos obrigatórios, a exibição do
  protocolo quando aplicável e a leitura do QR Code v3 impresso com
  dimensão mínima de 25 × 25 mm.
- [ ] Implementar e validar as duas vias em contingência off-line
  (`tpEmis=9`), com uma via para o consumidor e outra para a empresa
  até a transmissão e autorização da NF-e.

## NT 2014.002 v1.40

- [ ] Homologar a distribuição no Ambiente Nacional com e-CPF e e-CNPJ,
  inclusive CNPJ alfanumérico, e confirmar as três modalidades de consulta.
- [ ] Confirmar a distribuição dos eventos de prorrogação e cancelamento de
  prorrogação com respostas reais e importar o XML pelo leitor da NT 2015.001.

## NT 2026.004 v1.01

- [x] Confirmar localmente que o XSD principal aceita CNPJ e chave
  alfanuméricos e que a biblioteca preserva o CNPJ do destinatário na geração
  e reimportação de NF-e e NFC-e. Reproduzir a recusa na geração com emitente
  alfanumérico e no cancelamento com chave alfanumérica.
- [x] Ajustar a geração da chave de NF-e/NFC-e e as validações de cancelamento,
  substituição, manifestação e prorrogação. Aplicar o módulo 11 com o valor
  ASCII menos 48 no DV da chave, mantendo a compatibilidade com CNPJ numérico
  e CPF. Conferir também o identificador do pedido de prorrogação cancelado.
- [x] Com certificado local, validar NF-e e NFC-e com emitente alfanumérico,
  QR Code v3 em emissão normal e contingência, XSD da consulta de protocolo,
  manifestação e pedido de prorrogação. Confirmar assinatura de cancelamento,
  substituição e cancelamento de pedido, com bloqueio posterior nos XSDs locais.
- [x] Comparar o PL 010d v1.03 com os XSDs locais. O layout genérico de
  eventos é idêntico e validou o envelope dos XMLs assinados de cancelamento,
  substituição e cancelamento de prorrogação com chave alfanumérica.
  O pacote não inclui os layouts específicos desses eventos, do EPEC nem
  do Ator Interessado.
- [ ] Na próxima revisão de schemas, localizar as versões oficiais dos
  layouts específicos de cancelamento, substituição, Ator Interessado,
  EPEC e cancelamento de prorrogação. Conferir o `Id` e o identificador
  do pedido cancelado com chave alfanumérica, além do schema usado no
  retorno do EPEC e da leitura da lista de chaves pendentes.
  O `envEPEC_v1.00.xsd` local tem `TCOrgaoIBGE` duplicado e espera
  os totais dentro de `dest`, em desacordo com a NT 2014.001 v1.41.
- [ ] Validar com chaves alfanuméricas os retornos de autorização e de
  consulta de protocolo e os eventos com os schemas compatíveis.
  Homologar com a SEFAZ quando autorizado.
- [ ] Quando o DANFE entrar no escopo, avaliar o código de barras de chaves
  alfanuméricas conforme a seção 6 da NT Conjunta DFe 2025.001. O DANFE
  usa Code 128C fixo para a chave de acesso.

## NT 2024.003 v1.10

- [x] Mapear o grupo ZF em NF-e e NFC-e, com leitura, geração, limpeza e
  validação local do XML pelo XSD.
- [ ] Homologar com a SEFAZ as regras de receituário e CPF do responsável
  técnico, as exigências de guia por UF, NCM, CFOP e finalidade, e os retornos
  de guia inválida ou já utilizada.
- [ ] Confirmar em homologação o tratamento de destinatário estrangeiro na
  operação interestadual e as exceções de CSOSN 900 para o MEI.

## NT 2020.001 v1.60

- [x] Gerar, assinar e validar os quatro eventos de manifestação do
  destinatário e montar o XML processado para os retornos 135 e 136.
- [ ] Homologar no Ambiente Nacional com e-CPF e e-CNPJ reais, incluindo
  CNPJ-base, retificação, duplicidade, prazos de 10 e 90 dias, Ciência
  após manifestação conclusiva e os retornos 135 e 136.
- [ ] Conferir a procedência do pacote específico de schemas de manifestação.
  O envelope local tem uma definição duplicada de `TCOrgaoIBGE` e o schema
  de retorno tem erro de codificação. A biblioteca valida o envelope e o
  retorno com os schemas comuns e valida o detalhe por tipo de evento.

## NT 2022.002 v1.30a

- [x] Mapear diferimento e benefício fiscal no CST 90, partilha com CST 20
  e desoneração em `ICMSPart`, destinatário estrangeiro, transporte e
  importação de `refNFeSig`. Validar geração e reimportação com o XSD local.
- [ ] Homologar as regras E03a-10, E12-10, E14-10, E16a-20 e X04-10 com a
  SEFAZ, incluindo os CFOPs 7501, 7552 e 7667 e a combinação de país,
  UF de consumo, documento, IE e transportador. Confirmar a aplicação de
  BA02a-10 para `refNFeSig` em NF-e normal, de ajuste e nas demais finalidades.
- [ ] Homologar diferimento de ICMS/FCP no CST 90 e desoneração pelos
  motivos 10 e 11 em `ICMS20` e `ICMSPart`. Confirmar a procedência dos
  schemas locais frente ao pacote oficial PL 010c.

## NT 2023.002 v1.01

- [x] Validar CPF, série e processo de emissão na geração da NFC-e, aplicar
  a exceção de emissão avulsa de SC, limitar o lote a uma NFC-e e conferir
  o documento do autor do cancelamento com a chave.
- [ ] Homologar a emissão e o cancelamento com IE vinculada e e-CPF real,
  a NFC-e avulsa de SC com credenciais do Fisco, o tratamento da NFF com a
  SVRS e a rejeição 781 por irregularidade cadastral do emitente.
- [ ] Confirmar a procedência dos schemas usados na NFC-e e nos eventos e
  conferir as restrições a caracteres inválidos nos pacotes oficiais.

## NT 2023.002 v1.02

- [ ] Adequar o envio à autorização exclusivamente síncrona de NF-e e NFC-e
  (GAP03a-6, rejeição 129), prevista até 01/02/2027 em homologação e em
  05/04/2027 em produção. O lote unitário já envia `indSinc=1`. Definir
  como migrar os chamadores que enviam várias NF-e em um lote e validar
  a transição do fluxo que consulta recibo.
- [ ] Atualizar a consulta de URLs de inutilização para NF-e e NFC-e de
  AC, BA e SP conforme a desativação prevista para 11/11/2026. Preservar
  o endereço da SVRS para as demais UFs atendidas por ela.

## NT 2025.001 v1.03

- [x] Gerar QR Code versão 3 on-line e em contingência, mapear
  `tPag=91` e solicitar resposta síncrona para lote unitário.
- [ ] Homologar a consulta do QR Code por UF e ambiente, o uso de certificado
  real na contingência, a identificação do destinatário e a regra para
  emitente CPF, incluindo a exceção do Paraná.
- [ ] Confirmar a autorização síncrona para lote unitário. Homologar o prazo
  de emissão e os retornos
  100/150, as regras de IE do destinatário (incluindo ES e RJ), cobrança e
  pagamento, com atenção às exceções e às vigências por modelo e UF.

## NT 2019.001 v1.70

- [x] Mapear `gCred` e `ICMS51/cBenefRBC` no XML, preservar zeros informados
  no diferimento e zeros à esquerda em `cNF`, e conferir `modBCST=6` nos
  CSTs 10 e 30.
- [ ] Homologar as regras de benefício fiscal por UF, modelo, vigência e
  finalidade, com foco no cronograma de SP e SC, nas tabelas de cBenef por
  CST, em I05h-10, N14a-10/N14a-20 e na exceção de I05g-10 para NFF
  (`tpEmis=3`).
- [ ] Confirmar com a SEFAZ as rejeições dos cenários de diferimento, ICMS-ST,
  desoneração, CFOP e ISSQN, documentos referenciados, sequência de itens,
  código numérico, cadastro do destinatário e EPEC. A validação local
  verifica o leiaute pelo XSD.

## NT 2018.005 v1.52

- [x] Corrigir o XML de retirada/entrega, ICMS-ST, responsável técnico e
  mensagens do protocolo. Conferir também o mapeamento de isenção da ANVISA.
- [ ] Homologar no Paraná o CSRT do modelo 55 com credenciais reais, incluindo
  as exceções de emissão e os retornos das regras ZD e 7ZD. Conferir as regras
  de identificação do responsável técnico por UF.
- [ ] Confirmar com respostas reais da SEFAZ as mensagens no protocolo e o
  protocolo devolvido na rejeição 204. Tratar a preservação da assinatura do
  Fisco ao montar o XML processado a partir da resposta.
- [ ] Homologar as regras de cadastro de retirada/entrega e os cenários de
  complemento e restituição de ICMS-ST por UF e operação.
- [ ] Avaliar as alterações do DANFE dos itens 5.1 a 5.3 quando o componente
  entrar no escopo.

## NT 2023.001 v1.60

- [x] Suporte local aos CSTs 02/15/53/61, pBio, origComb e totais implementado
  no commit `641f3fd`, com validação estrutural, ida e volta e assinaturas.
- [ ] Avaliar a exibição de infAdProd no DANFE quando esse componente entrar no escopo.
- [ ] Homologar com tabelas ANP e alíquotas aplicáveis à operação, exceções por
  modelo/finalidade/consumidor final e regras de totalização. Os testes locais
  não comprovam aceitação fiscal; não houve acesso à SEFAZ na implementação.
- [ ] Validar qmake/Windows quando esses ambientes entrarem no escopo.

Referência: [análise anterior à implementação](temp/analise-NT2023.001-v1.60-2026-11-04-22-54-11.md).

## Endereços NF-e/NFC-e

- [x] URLs e roteamentos corrigidos localmente no commit `476fe26`.
- [ ] Confirmar disponibilidade/autorização real dos serviços em homologação,
  especialmente SP/H, CE/SVRS, GO, cadastro, contingência, AN e consultas públicas.
- [ ] Em futura atualização da referência, conferir a precedência/limpeza das chaves
  duplicadas URL-QRCode de PI/PR/TO. Os valores mantidos estão presentes no INI.

Referência: [INI arquivado](temp/completed/ACBrNFeServicos.ini).

## NT 2015.001 v1.30

- [x] Pedidos 111500/111501, cancelamentos 111502/111503 e leitura das respostas
  411500–411503 implementados localmente no commit `35f251d`.
- [ ] Homologar na SEFAZ MG (solicitação completa) e SP (parcial), ambos os prazos e
  seus cancelamentos, autor/protocolo/certificado real, saldo de itens/quantidades,
  prazos, limites estaduais de pedidos pendentes (inclusive 638/639), respostas
  sucessivas e indeferimentos do Fisco (incluindo 809), ordem de cancelamento dos
  prazos e rejeição 811.
- [ ] Obter exemplos reais de respostas por item/solicitação completa e confrontar
  com o modelo/importador, preservando histórico por idPedido e sequência
  independente do Fisco.

## Inutilização de numeração de NF-e/NFC-e

- [x] Implementar `EventoNotaFiscal::inutNFe`, com `InutNFe`, `InfInut`
  compartilhado entre os leiautes de pedido e retorno e `RetInutNFe`.
  Validar geração, assinatura, XSD, processamento e gravação com transporte
  simulado, inclusive rejeições e preservação da assinatura do retorno.
- [ ] Homologar com a SEFAZ: modelos 55/65, UF/ambiente, certificado real,
  confirmação 102, duplicidade 563 com protocolo anterior, sobreposição de
  faixas e numeração já utilizada. Não houve transmissão real nesta etapa.
- [ ] Confirmar a procedência oficial dos schemas de inutilização e sua
  compatibilidade com CNPJ alfanumérico. A validação local utiliza o pacote
  de trabalho existente. A atualização de URLs de AC/BA/SP permanece na
  pendência da NT 2023.002 v1.02.

## Cancelamento da NF-e/NFC-e

- [x] CancNFe implementado para cancelamento normal 110111 e substituição NFC-e
  110112, com campos próprios, assinatura, schemas, RetEventoCancNFe, processado e
  gravação. EnvEvento recusa esses eventos.
- [ ] Homologar: modelo/UF, certificado real, prazo/fora de prazo, duplicidade,
  protocolo, contingência, CPF/CNPJ e impedimentos (incluindo 811).
- [ ] Validar qmake/Windows quando esses ambientes entrarem no escopo.
- [ ] Conferir no pacote oficial o nome do schema de retorno da substituição: o MOC cita
  retEventoCancSubst_v1.00.xsd e o pacote local traz retEnvEventoCancSubst_v1.00.xsd.

## Demais pendências

- [x] Manter pacote de schemas e dependências coerentes e confirmar sua procedência.
  Os oito detalhes da NT 2015.001 e schemas genéricos compilam; o pacote local
  RemIndus tem TCOrgaoIBGE duplicado. Para usar seus envelopes diretamente será
  necessário obter um conjunto oficial compatível. O fluxo implementado não
  depende deles.
- [ ] Continuar as principais NTs indicadas pelo mantenedor, atualizando os grupos existentes.
- [ ] Testar SEFAZ após atualização das principais NTs. NT 2023.004:
  SUFRAMA/indDeduzDeson, totalização W16-10, card/PIX por UF, data/documentos,
  troco e exceção NFA-e.
- [ ] ECONF/Ator/MEI com dados reais: prazos, habilitação CT-e, autorizações anteriores,
  CFOP/CSOSN por operação/modelo, GTIN/NCM/origem/DIFAL e rejeições cadastrais.
- [ ] HTTP/OpenSSL: getaddrinfo, porta/host e verificação do certificado do servidor.
- [ ] Remoção futura de Container::items, Windows/exemplos e validação com certificado real.


Manter builds/testes em build/ e o código do QtRPT fora do escopo.
