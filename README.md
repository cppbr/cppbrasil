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
- LibXML2
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

## Compilação
### Linux
A compilação no Linux pode ser feita via cmake ou utilizando o script MakeLinux.sh. O script utiliza o cmake e o ninja e será instalado dentro do diretório /usr.

### Windows
A compilação no Windows pode ser feita via cmake ou utilizando o script MakeMSVC.bat. A compilação é feita através do compilador MSVC que pode ser obtido através da instalação do visual studio. Vocẽ também precisa indicar, em Variáveis de Ambiente, o caminho do cmake e ninja que se encontram na instalação do Qt. Para windows foi disponibilizado os requisitos no diretório third-party. Caso você queira utilizar versões mais atualizadas, basta substituir os arquivos em third-party/windows. Caso seja alterado o nome da pasta, será necessário alterar em MakeMSVC.bat.


# Autor
João Elson.


**Site do Projeto e fórum para suporte, interação e sugestões**
- https://cppbrasil.com.br



