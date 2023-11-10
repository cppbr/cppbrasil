# Projeto C++ Brasil  
![GitHub](https://img.shields.io/github/license/cppbr/cppbrasil?color=blueviolet)

# Sobre o projeto

Projeto desenvolvido em C++ e Qt framework para criação de bibliotecas multiplataforma que ajudam no desenvolvimento de software ERP.


## Bibliotecas desenvolvidas
- libCppNFe - Biblioteca para emissão de documentos fiscais NFe e NFCe.
- libDanfeQtRpt - Biblioteca para impressão de DANFE(Documento auxiliar de Nota Fiscal Eletrônica). Utiliza o QtRpt(Gerenciador de Relatórios open source).


# Tecnologias utilizadas
- C/C++
- Qt Framework
## Ambientes testados
- Linux
- Windows
- Android

## Requisitos
- Qt >= 5.15.2
- OpenSSL
- LibXML2
- ZLib
- libIconv(caso opte por compilar a libxml2 com suporte a libIconv. No Linux vem instalado por padrão. Para Windows [link.](https://github.com/pffang/libiconv-for-Windows))

## Compilação
### Linux
A compilação no Linux pode ser feita via cmake ou utilizando o script MakeLinux.sh. O script utiliza o cmake e o ninja e será instalado dentro do diretório /usr.

### Windows
A compilação no Windows pode ser feita via cmake ou utilizando o script MakeMSVC.bat. A compilação é feita através do compilador MSVC que pode ser obtido através da instalação do visual studio. Vocẽ também precisa indicar, em Variáveis de Ambiente, o caminho do cmake e ninja que se encontram na instalação do Qt. Para windows foi disponibilizado os requisitos no diretório third-party. Caso você queira utilizar versões mais atualizadas, basta substituir os arquivos em third-party/windows. Caso seja alterado o nome da pasta, será necessário alterar em MakeMSVC.bat.


# Autor
João Elson.


**Site do Projeto e fórum para suporte, interação e sugestões**
- https://cppbrasil.com.br



