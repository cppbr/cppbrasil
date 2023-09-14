cmake_minimum_required(VERSION 3.14)

project(CppNFe LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)

#Caso necessário, informe os diretório dos Header Files openssl, libxml2, zlib e libIconv(caso esteja no windows)
#include_directories(
#/usr/include/openssl
#/usr/include/libxml2
#/usr/include/zlib
#/usr/include/libIconv
#)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(cppnfe_SRC
    ../cppbrasil_global.h
    ../config.h
    ../config.cpp
    ../container.h
    ../convdf.h
    ../convdf.cpp
    ../cppcrypto.h
    ../cppcrypto.cpp
    ../cpplibxml2.h
    ../cpplibxml2.cpp
    ../cppopenssl.h
    ../cppopenssl.cpp
    ../cpputility.h
    ../cpputility.cpp
    ../cppxml.h
    ../cppxml.cpp
    ../cppzip.h
    ../cppzip.cpp
    ../httpopenssl.h
    ../httpopenssl.cpp
    ../httpclient.h
    ../httpclient.cpp
    ../httpqt.h
    ../httpqt.cpp
    ../webservices.h
    ../webservices.cpp
    resourcenfe.qrc
    cppnfe.h
    cppnfe.cpp
    adi.h
    adi.cpp
    arma.h
    arma.cpp
    autxml.h
    autxml.cpp
    avulsa.h
    avulsa.cpp
    cana.h
    cana.cpp
    card.h
    card.cpp
    cide.h
    cide.cpp
    cobr.h
    cobr.cpp
    cofins.h
    cofins.cpp
    cofinsst.h
    cofinsst.cpp
    comb.h
    comb.cpp
    compra.h
    compra.cpp
    confignfe.h
    confignfe.cpp
    conssitnfe.h
    conssitnfe.cpp
    convnf.h
    convnf.cpp
    deduc.h
    deduc.cpp
    dest.h
    dest.cpp
    det.h
    det.cpp
    detexport.h
    detexport.cpp
    detpag.h
    detpag.cpp
    di.h
    di.cpp
    dup.h
    dup.cpp
    emit.h
    emit.cpp
    encerrante.h
    encerrante.cpp
    enderdest.h
    enderdest.cpp
    enderemit.h
    enderemit.cpp
    entrega.h
    entrega.cpp
    envevento.h
    envevento.cpp
    tevento.h
    tevento.cpp
    eventonotafiscal.h
    eventonotafiscal.cpp
    exporta.h
    exporta.cpp
    exportind.h
    exportind.cpp
    fat.h
    fat.cpp
    fordia.h
    fordia.cpp
    icms.h
    icms.cpp
    icmstot.h
    icmstot.cpp
    icmsufdest.h
    icmsufdest.cpp
    ide.h
    ide.cpp
    ii.h
    ii.cpp
    imposto.h
    imposto.cpp
    impostodevol.h
    impostodevol.cpp
    infadic.h
    infadic.cpp
    infintermed.h
    infintermed.cpp
    infnfe.h
    infnfe.cpp
    infnfesupl.h
    infnfesupl.cpp
    infresptec.h
    infresptec.cpp
    ipi.h
    ipi.cpp
    issqn.h
    issqn.cpp
    issqntot.h
    issqntot.cpp
    lacres.h
    lacres.cpp
    med.h
    med.cpp
    nfe.h
    nfe.cpp
    nfref.h
    nfref.cpp
    notafiscal.h
    notafiscal.cpp
    obscont.h
    obscont.cpp
    obsfisco.h
    obsfisco.cpp
    obsitem.h
    obsitem.cpp
    origcomb.h
    origcomb.cpp
    pag.h
    pag.cpp
    pis.h
    pis.cpp
    pisst.h
    pisst.cpp
    procref.h
    procref.cpp
    prod.h
    prod.cpp
    rastro.h
    rastro.cpp
    reboque.h
    reboque.cpp
    refecf.h
    refecf.cpp
    refnf.h
    refnf.cpp
    refnfp.h
    refnfp.cpp
    retconssitnfe.h
    retconssitnfe.cpp
    retconsstatserv.h
    retconsstatserv.cpp
    retenvevento.h
    retenvevento.cpp
    retirada.h
    retirada.cpp
    retornonfe.h
    retornonfe.cpp
    rettransp.h
    rettransp.cpp
    rettrib.h
    rettrib.cpp
    statusservico.h
    statusservico.cpp
    total.h
    total.cpp
    transp.h
    transp.cpp
    transporta.h
    transporta.cpp
    urlnfe.h
    urlnfe.cpp
    veicprod.h
    veicprod.cpp
    veictransp.h
    veictransp.cpp
    vol.h
    vol.cpp
    wsnfe.h
    wsnfe.cpp
    xmlread.h
    xmlread.cpp
    xmlwrite.h
    xmlwrite.cpp
)

set(HEADER_GERAL
    ../cppbrasil_global.h
    ../config.h
    ../container.h
    ../convdf.h
    ../cppcrypto.h
    ../cpplibxml2.h
    ../cppopenssl.h
    ../cpputility.h
    ../cppxml.h
    ../cppzip.h
    ../httpopenssl.h
    ../httpclient.h
    ../httpqt.h
    ../webservices.h
    ../ConvDF
    ../CppUtil
)
set(HEADER_NFE
    cppnfe.h
    adi.h
    arma.h
    autxml.h
    avulsa.h
    cana.h
    card.h
    cide.h
    cobr.h
    cofins.h
    cofinsst.h
    comb.h
    compra.h
    confignfe.h
    conssitnfe.h
    convnf.h
    deduc.h
    dest.h
    det.h
    detexport.h
    detpag.h
    di.h
    dup.h
    emit.h
    encerrante.h
    enderdest.h
    enderemit.h
    entrega.h
    envevento.h
    eventonotafiscal.h
    exporta.h
    exportind.h
    fat.h
    fordia.h
    icms.h
    icmstot.h
    icmsufdest.h
    ide.h
    ii.h
    imposto.h
    impostodevol.h
    infadic.h
    infintermed.h
    infnfe.h
    infnfesupl.h
    infresptec.h
    ipi.h
    issqn.h
    issqntot.h
    lacres.h
    med.h
    nfe.h
    nfref.h
    notafiscal.h
    obscont.h
    obsfisco.h
    obsitem.h
    origcomb.h
    pag.h
    pis.h
    pisst.h
    procref.h
    prod.h
    rastro.h
    reboque.h
    refecf.h
    refnf.h
    refnfp.h
    retconsstatserv.h
    retenvevento.h
    retirada.h
    retornonfe.h
    rettransp.h
    rettrib.h
    statusservico.h
    total.h
    transp.h
    transporta.h
    urlnfe.h
    veicprod.h
    veictransp.h
    vol.h
    wsnfe.h
    xmlread.h
    xmlwrite.h
    ConfigNFe
    ConvNF
    CppNFe
)


add_library(CppNFe SHARED
    ${cppnfe_SRC}
)

target_link_libraries(CppNFe PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(CppNFe PRIVATE Qt${QT_VERSION_MAJOR}::Network)

if (CMAKE_SYSTEM MATCHES "Windows")
    target_link_libraries(CppNFe PRIVATE wsock32 ws2_32)
endif()

target_compile_definitions(CppNFe PRIVATE CPPNFE_LIBRARY)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if (CMAKE_SYSTEM MATCHES "Linux")
        set(CMAKE_INSTALL_PREFIX
            "/usr" CACHE PATH "CppBrasil default install prefix" FORCE)
    endif()         
endif()

install(TARGETS CppNFe
    LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_property(TARGET CppNFe PROPERTY PUBLIC_HEADER ${HEADER_GERAL})
install(TARGETS CppNFe
    PUBLIC_HEADER DESTINATION include/CppBrasil COMPONENT devel
)
set_property(TARGET CppNFe PROPERTY PUBLIC_HEADER ${HEADER_NFE})
install(TARGETS CppNFe
        PUBLIC_HEADER DESTINATION include/CppBrasil/NFe COMPONENT devel
)

#caso necessário, informe os diretórios das libs openssl, libxml2, zlib e libIconv(caso esteja no windows). 
#target_link_directories(CppNFe
#    PRIVATE "/home/usuario/openssl/"
#    PRIVATE "/home/usuario/libxml2"
#    PRIVATE "/home/usuario/zlib"
#    PRIVATE "/home/usuario/libiconv" 
#)

#link libs no windows ou linux
if (CMAKE_SYSTEM MATCHES "Windows")
    target_link_libraries(CppNFe  PRIVATE libcrypto libssl libiconv libxml2 zdll)
elseif (CMAKE_SYSTEM MATCHES "Linux")
    target_link_libraries(CppNFe  PRIVATE -lcrypto -lssl  -lxml2 -lz)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin") #Darwin para Mac OS
    target_link_libraries(CppNFe  PRIVATE crypto ssl  xml2 z)
endif()
