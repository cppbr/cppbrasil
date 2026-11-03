# Projeto C++ Brasil  
![GitHub](https://img.shields.io/github/license/cppbr/cppbrasil?color=blueviolet)

# Sobre o projeto

Projeto desenvolvido em C++ e Qt framework para criação de bibliotecas multiplataforma que ajudam no desenvolvimento de software ERP.


## Bibliotecas desenvolvidas
- libCppNFe - Biblioteca para emissão de documentos fiscais NFe e NFCe.
- libDanfeQtRpt - Biblioteca para impressão de DANFE(Documento auxiliar de Nota Fiscal Eletrônica). Utiliza o QtRpt(Gerenciador de Relatórios open source).

## Mudanças
Veja o [CHANGELOG](CHANGELOG.md) para as mudanças de cada versão, inclusive as que quebram compatibilidade.


# Tecnologias utilizadas
- C/C++
- Qt Framework
## Ambientes testados
- Linux
- Windows
- Android

## Requisitos
- Qt >= 5.15.2
- OpenSSL >= 1.1.1 (recomendado 3.x)
- LibXML2 (testada com 2.12 e 2.15; recomendado **2.13 ou superior**, ver abaixo)
- ZLib
- libIconv(caso opte por compilar a libxml2 com suporte a libIconv. No Linux vem instalado por padrão. Para Windows [link.](https://github.com/pffang/libiconv-for-Windows))

### Fedora/RHEL e derivados: assinatura SHA1
A NF-e é assinada com **RSA-SHA1**, e o OpenSSL do Fedora/RHEL (e derivados, como CentOS Stream, Rocky, Alma e Nobara)
bloqueia assinaturas SHA1 por padrão. Sem liberar, `assinar()` falha com "Erro ao assinar documento"
(`evp_pkey_ctx_set_md: invalid digest`). Libere de uma das formas:

- No sistema todo: `sudo update-crypto-policies --set DEFAULT:SHA1` (para voltar: `--set DEFAULT`).
- Só na aplicação: aponte a variável `OPENSSL_CONF` para um arquivo como este:

```ini
openssl_conf = openssl_init

[openssl_init]
alg_section = evp_properties

[evp_properties]
rh-allow-sha1-signatures = yes
```

Windows, Debian e Ubuntu não têm esse bloqueio.

### LibXML2: versão e recompilação
- **2.13 ou superior (recomendado):** erros de leitura dos XSDs incluídos pelo schema principal (por exemplo, um
  `leiauteNFe_v4.00.xsd` corrompido) aparecem na mensagem de erro da validação. Na 2.12 ou anterior, esse erro também é
  impresso no stderr da aplicação.
- **2.14 ou superior: o arquivo da biblioteca mudou** de `libxml2.so.2` para `libxml2.so.16` (no Windows, a DLL também
  pode mudar de nome). Ao atualizar a libxml2 a partir da 2.13 ou anterior, **recompile a CppBrasil e a sua aplicação**
  contra a versão nova; os binários compilados com a antiga não carregam a nova.
- A lib funciona com a libxml2 compilada sem a interface antiga SAX1 (`--without-sax1`).

## Evento ECONF (NT 2024.002 v1.00)
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

## Ator interessado na NF-e — Transportador (NT 2020.007 v1.40)
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

## Compilação
### Linux
A compilação no Linux pode ser feita via cmake ou utilizando o script MakeLinux.sh. O script utiliza o cmake e o ninja e será instalado dentro do diretório /usr.

### Windows
A compilação no Windows pode ser feita via cmake ou utilizando o script MakeMSVC.bat. A compilação é feita através do compilador MSVC que pode ser obtido através da instalação do visual studio. Vocẽ também precisa indicar, em Variáveis de Ambiente, o caminho do cmake e ninja que se encontram na instalação do Qt. Para windows foi disponibilizado os requisitos no diretório third-party. Caso você queira utilizar versões mais atualizadas, basta substituir os arquivos em third-party/windows. Caso seja alterado o nome da pasta, será necessário alterar em MakeMSVC.bat.


# Autor
João Elson.


**Site do Projeto e fórum para suporte, interação e sugestões**
- https://cppbrasil.com.br



