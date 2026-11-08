find_package(Qt6 REQUIRED COMPONENTS Gui Svg Widgets)

set(cppdanfe_HEADERS
    code128.h
    cppbarcode.h
    cppdanfe.h
    qrcodegen.hpp
)

add_library(CppDanfe SHARED
    ${cppdanfe_HEADERS}
    code128.cpp
    cppbarcode.cpp
    cppdanfe.cpp
    qrcodegen.cpp
)

target_compile_definitions(CppDanfe PRIVATE CPPDANFE_LIBRARY)

target_link_libraries(CppDanfe PUBLIC
    CppNFe
    Qt6::Gui
    Qt6::Svg
    Qt6::Widgets
)

install(TARGETS CppDanfe)
install(FILES
    ${cppdanfe_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/CppDanfe
    COMPONENT devel
)
