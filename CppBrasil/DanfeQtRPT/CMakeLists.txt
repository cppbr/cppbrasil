cmake_minimum_required(VERSION 3.14)

project(CppDanfeQtRPT LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Gui Xml Charts Qml Sql Svg PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Gui Xml Charts Qml Sql Svg PrintSupport)

#Caso necessário, informe os diretório dos Header Files openssl, libxml2, zlib, libIconv(caso esteja no windows), libCppNFe e libQtRPT
#include_directories(
#/usr/include/openssl
#/usr/include/libxml2
#/usr/include/zlib
#/usr/include/libIconv
#/usr/include/CppBrasil/NFe
#/usr/include/QtRPT
#)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(cppdanfe_SRC
    ../cppbrasil_global.h
    ../code128.h
    ../code128.cpp
    ../cppbarcode.h
    ../cppbarcode.cpp
    ../cppdanfe.h
    ../cppdanfe.cpp
    ../qrcodegen.hpp
    ../qrcodegen.cpp
    cppdanfeqtrpt.h
    cppdanfeqtrpt.cpp
)

set(HEADER_GERAL
    ../cppbrasil_global.h
    ../code128.h
    ../cppbarcode.h
    ../cppdanfe.h
    ../qrcodegen.hpp
)

set(HEADER_DANFE
    cppdanfeqtrpt.h
    CppDanfeQtRPT
)

add_library(CppDanfeQtRPT SHARED
    ${cppbrasil_GERAL}
    ${cppdanfe_SRC}
)

target_link_libraries(CppDanfeQtRPT PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Svg
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

target_compile_definitions(CppDanfeQtRPT PRIVATE CPPBRASIL_LIBRARY)

install(TARGETS CppDanfeQtRPT
    LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_property(TARGET CppDanfeQtRPT PROPERTY PUBLIC_HEADER ${HEADER_GERAL})
install(TARGETS CppDanfeQtRPT
    PUBLIC_HEADER DESTINATION include/CppBrasil COMPONENT devel
)
set_property(TARGET CppDanfeQtRPT PROPERTY PUBLIC_HEADER ${HEADER_DANFE})
install(TARGETS CppDanfeQtRPT
        PUBLIC_HEADER DESTINATION include/CppBrasil/DanfeQtRPT COMPONENT devel
)

#caso necessário, informe os diretórios das libs openssl, libxml2, zlib, libIconv(caso esteja no windows), libCppNFe e libQtRPT
#target_link_directories(CppNFe
#    PRIVATE "/home/usuario/openssl/"
#    PRIVATE "/home/usuario/libxml2"
#    PRIVATE "/home/usuario/zlib"
#    PRIVATE "/home/usuario/libiconv" 
#    PRIVATE "/home/usuario/CppBrasil/lib"
#    PRIVATE "/home/usuario/QtRptProject/lib"
#)
#link libs no windows, linux e macos
if (CMAKE_SYSTEM MATCHES "Windows")
    target_link_libraries(CppNFe  PRIVATE libCppNFe libQtRPT libcrypto libssl libiconv.lib libxml2 zlib )
elseif (CMAKE_SYSTEM MATCHES "Linux")
    target_link_libraries(CppNFe  PRIVATE -lCppNFe -lQtRPT -lcrypto -lssl  -lxml2 -lz)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin") #Darwin para Mac OS
    target_link_libraries(CppNFe  PRIVATE CppNFe QtRPT crypto ssl  xml2 z)
endif()

