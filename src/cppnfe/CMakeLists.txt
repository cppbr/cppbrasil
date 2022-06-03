cmake_minimum_required(VERSION 3.14)

project(cppnfe LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(cppnfe  SHARED #STATIC
    resourcenfe.qrc
    cppnfe.h
    cppnfe.cpp
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
    eventos.h 
    eventos.cpp
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
    retconsstatserv.h 
    retconsstatserv.cpp
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

target_link_libraries(cppnfe PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(cppnfe PRIVATE Qt${QT_VERSION_MAJOR}::Network)

target_compile_definitions(cppnfe PRIVATE CPPBRASIL_LIBRARY)

#para bibliotecas estática descomente a linha abaixo
#target_compile_definitions(cppnfe PRIVATE CPPBRASIL_STATIC)

file(GLOB CPPBRASIL_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/../*.h)
file(GLOB CPPNFE_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

install(TARGETS cppnfe
    LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES ${CPPBRASIL_FILES}
        DESTINATION  ${CMAKE_INSTALL_PREFIX}/include/cppbrasil
)
install(FILES ${CPPNFE_FILES}
        DESTINATION  ${CMAKE_INSTALL_PREFIX}/include/cppbrasil
)



        
