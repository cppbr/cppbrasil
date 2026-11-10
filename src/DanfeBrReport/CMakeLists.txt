find_package(Qt6 REQUIRED COMPONENTS Gui Widgets PrintSupport)

#O BrReport não acompanha o projeto: https://github.com/cppbr/BrReport
find_package(BrReport QUIET)

if(NOT BrReport_FOUND)
    message(FATAL_ERROR "BrReport não encontrado. Informe a pasta da instalação em CMAKE_PREFIX_PATH "
                        "ou compile sem o DANFE com -DCPPBRASIL_BUILD_DANFE=OFF")
endif()

add_library(CppDanfeBrReport SHARED
    cppdanfebrreport.h
    cppdanfebrreport.cpp
)

target_compile_definitions(CppDanfeBrReport PRIVATE CPPDANFEBRREPORT_LIBRARY)

#O BrReport é usado somente na implementação: quem usa a CppDanfeBrReport não precisa dos headers dele.
target_link_libraries(CppDanfeBrReport
    PUBLIC
        CppDanfe
    PRIVATE
        BrReport::Widgets
)

install(TARGETS CppDanfeBrReport)
install(FILES
    cppdanfebrreport.h
    CppDanfeBrReport
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/DanfeBrReport
    COMPONENT devel
)
