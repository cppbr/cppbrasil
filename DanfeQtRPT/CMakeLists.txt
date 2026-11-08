find_package(Qt6 REQUIRED COMPONENTS Xml Charts Qml Sql PrintSupport)

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
    Qt6::Xml
    Qt6::Charts
    Qt6::Qml
    Qt6::Sql
    Qt6::PrintSupport
)

install(TARGETS CppDanfeQtRPT)
install(FILES
    cppdanfeqtrpt.h
    CppDanfeQtRPT
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/DanfeQtRPT
    COMPONENT devel
)
