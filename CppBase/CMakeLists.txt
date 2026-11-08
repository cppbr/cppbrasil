find_package(LibXml2 REQUIRED)

#Caso necessário, informe os diretório dos Header Files openssl, libxml2, zlib e libIconv(caso esteja no windows)
#include_directories(
#/usr/include/openssl
#/usr/include/libxml2
#/usr/include/zlib
#/usr/include/libIconv
#)

set(cppbase_HEADERS
    cppbrasil_global.h
    config.h
    container.h
    convdf.h
    cppcrypto.h
    cpplibxml2.h
    cppopenssl.h
    cpputility.h
    cppxml.h
    cppzip.h
    httpclient.h
    httpopenssl.h
    httpqt.h
    webservices.h
)

add_library(CppBase SHARED
    ${cppbase_HEADERS}
    config.cpp
    convdf.cpp
    cppcrypto.cpp
    cpplibxml2.cpp
    cppopenssl.cpp
    cpputility.cpp
    cppxml.cpp
    cppzip.cpp
    httpclient.cpp
    httpopenssl.cpp
    httpqt.cpp
    webservices.cpp
)

target_compile_definitions(CppBase PRIVATE CPPBASE_LIBRARY)

target_include_directories(CppBase PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${LIBXML2_INCLUDE_DIR}
)

target_link_libraries(CppBase PUBLIC
    Qt6::Core
    Qt6::Network
)

#caso necessário, informe os diretórios das libs openssl, libxml2, zlib e libIconv(caso esteja no windows).
#target_link_directories(CppBase
#    PUBLIC "/home/usuario/openssl/"
#    PUBLIC "/home/usuario/libxml2"
#    PUBLIC "/home/usuario/zlib"
#    PUBLIC "/home/usuario/libiconv"
#)

#link libs no windows ou linux
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
    target_link_libraries(CppBase PUBLIC wsock32 ws2_32 OpenSSL::SSL OpenSSL::Crypto
                          LibXml2::LibXml2 ${CPPBASE_ZLIB})
elseif (CMAKE_SYSTEM MATCHES "Linux")
    target_link_libraries(CppBase PUBLIC -lcrypto -lssl -lxml2 -lz)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin") #Darwin para Mac OS
    target_link_libraries(CppBase PUBLIC crypto ssl xml2 z)
endif()

install(TARGETS CppBase)
install(FILES
    ${cppbase_HEADERS}
    ConvDF
    CppUtil
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/CppBase
    COMPONENT devel
)
