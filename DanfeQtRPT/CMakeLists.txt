find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Xml Charts Qml Sql PrintSupport)

#O QtRPT não acompanha o projeto. O código-fonte está em QtRptProject-Source
find_path(QTRPT_INCLUDE_DIR NAMES qtrpt.h PATH_SUFFIXES QtRPT)
find_library(QTRPT_LIBRARY NAMES QtRPT)

if(NOT QTRPT_INCLUDE_DIR OR NOT QTRPT_LIBRARY)
    message(FATAL_ERROR "QtRPT não encontrado. Informe -DQTRPT_INCLUDE_DIR e -DQTRPT_LIBRARY "
                        "ou compile sem o DANFE com -DCPPBRASIL_BUILD_DANFE=OFF")
endif()

add_library(CppDanfeQtRPT SHARED
    cppdanfeqtrpt.h
    cppdanfeqtrpt.cpp
)

target_compile_definitions(CppDanfeQtRPT PRIVATE CPPDANFEQTRPT_LIBRARY)

target_include_directories(CppDanfeQtRPT PUBLIC ${QTRPT_INCLUDE_DIR})

target_link_libraries(CppDanfeQtRPT PUBLIC
    CppDanfe
    ${QTRPT_LIBRARY}
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

install(TARGETS CppDanfeQtRPT)
install(FILES
    cppdanfeqtrpt.h
    CppDanfeQtRPT
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/DanfeQtRPT
    COMPONENT devel
)
