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
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
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
    target_link_libraries(CppBase PUBLIC wsock32 ws2_32 libcrypto libssl libiconv libxml2 zdll)
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
