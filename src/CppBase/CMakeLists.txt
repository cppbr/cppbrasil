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

#A CppBase não gera biblioteca própria: os fontes comuns são compilados dentro da
#CppNFe, que resulta em uma única biblioteca (CppNFe.dll / libCppNFe.so).
target_sources(CppNFe PRIVATE
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

target_include_directories(CppNFe PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${LIBXML2_INCLUDE_DIR}
)

target_link_libraries(CppNFe PUBLIC
    Qt6::Core
    Qt6::Network
)

#caso necessário, informe os diretórios das libs openssl, libxml2, zlib e libIconv(caso esteja no windows).
#target_link_directories(CppNFe
#    PUBLIC "/home/usuario/openssl/"
#    PUBLIC "/home/usuario/libxml2"
#    PUBLIC "/home/usuario/zlib"
#    PUBLIC "/home/usuario/libiconv"
#)

#link libs no windows ou linux
if (CMAKE_SYSTEM MATCHES "Windows")
    #OpenSSL, libxml2 e zlib são localizados em src/CMakeLists.txt (CPPBASE_ZLIB)
    target_link_libraries(CppNFe PUBLIC wsock32 ws2_32 OpenSSL::SSL OpenSSL::Crypto
                          LibXml2::LibXml2 ${CPPBASE_ZLIB})
elseif (CMAKE_SYSTEM MATCHES "Linux")
    target_link_libraries(CppNFe PUBLIC -lcrypto -lssl -lxml2 -lz)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin") #Darwin para Mac OS
    target_link_libraries(CppNFe PUBLIC crypto ssl xml2 z)
endif()

install(FILES
    ${cppbase_HEADERS}
    ConvDF
    CppUtil
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/CppBase
    COMPONENT devel
)
