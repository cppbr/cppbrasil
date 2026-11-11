cmake_minimum_required(VERSION 3.14)

project(CppBrasil LANGUAGES CXX)

option(CPPBRASIL_BUILD_DANFE "Compila a CppDanfe e a CppDanfeBrReport, impressão do DANFE com o BrReport" ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if (CMAKE_SYSTEM MATCHES "Linux")
        set(CMAKE_INSTALL_PREFIX
            "/usr" CACHE PATH "CppBrasil default install prefix" FORCE)
    endif()
endif()

include(GNUInstallDirs)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network)

#Dependências dos arquivos comuns (CppBase), compilados dentro da CppNFe. Ficam aqui porque
#os alvos importados do find_package só são visíveis na pasta em que foram localizados.
find_package(LibXml2 REQUIRED)
if (CMAKE_SYSTEM MATCHES "Windows")
    #informe os diretórios em CMAKE_PREFIX_PATH (libxml2) e OPENSSL_ROOT_DIR (OpenSSL).
    #O libiconv só é necessário se a libxml2 tiver sido compilada com ele; o alvo
    #LibXml2::LibXml2 já traz as dependências da própria libxml2.
    find_package(OpenSSL 1.1.1 REQUIRED)
    find_package(ZLIB QUIET)
    if (ZLIB_FOUND)
        set(CPPBASE_ZLIB ZLIB::ZLIB)
    else()
        #sem zlib instalada, usa a zlib que acompanha o Qt
        find_package(Qt6 REQUIRED COMPONENTS ZlibPrivate)
        set(CPPBASE_ZLIB Qt6::ZlibPrivate)
    endif()
endif()

#A CppNFe é criada primeiro; a CppBase acrescenta a ela os fontes comuns (biblioteca única)
add_subdirectory(CppNFe)
add_subdirectory(CppBase)

if(CPPBRASIL_BUILD_DANFE)
    add_subdirectory(CppDanfe)
    add_subdirectory(DanfeBrReport)
endif()
