find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Xml Charts Qml Sql PrintSupport)

#QtRPT: por padrão usa a cópia de third-party. Para usar outra, informe -DQTRPT_INCLUDE_DIR e -DQTRPT_LIBRARY
if(CMAKE_SYSTEM MATCHES "Windows")
    set(QTRPT_ROOT ${PROJECT_SOURCE_DIR}/third-party/windows/QtRptProject3.0)
else()
    set(QTRPT_ROOT ${PROJECT_SOURCE_DIR}/third-party/linux/QtRptProject3.0)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(QTRPT_LIB_DIR ${QTRPT_ROOT}/lib/debug)
else()
    set(QTRPT_LIB_DIR ${QTRPT_ROOT}/lib/release)
endif()

find_path(QTRPT_INCLUDE_DIR NAMES qtrpt.h HINTS ${QTRPT_ROOT}/include/QtRPT)
find_library(QTRPT_LIBRARY NAMES QtRPT HINTS ${QTRPT_LIB_DIR} ${QTRPT_LIB_DIR}/lib)

if(NOT QTRPT_INCLUDE_DIR OR NOT QTRPT_LIBRARY)
    message(FATAL_ERROR "QtRPT não encontrado. Informe QTRPT_INCLUDE_DIR e QTRPT_LIBRARY "
                        "ou desative o DANFE com -DCPPBRASIL_BUILD_DANFE=OFF")
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
