cmake_minimum_required(VERSION 3.21)

project(CppBrasilExamples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

option(CPPBRASIL_EXAMPLES_WIDGETS "Compila o exemplo Qt Widgets" ON)

# O exemplo compila a CppBrasil junto, a partir de src/. Para usar outra cópia
# dos fontes, informe -DCPPBRASIL_SRC=<pasta src da CppBrasil>.
set(CPPBRASIL_SRC "${CMAKE_CURRENT_LIST_DIR}/../src" CACHE PATH "Pasta src da CppBrasil")
set(CPPBRASIL_BUILD_DANFE OFF CACHE BOOL "" FORCE)
add_subdirectory("${CPPBRASIL_SRC}" cppbrasil)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network)

add_library(exemploNFe STATIC nfe.cpp nfe.h)
target_include_directories(exemploNFe PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(exemploNFe PUBLIC Qt6::Core Qt6::Network CppNFe)

add_executable(consoleNFe console/main.cpp)
target_link_libraries(consoleNFe PRIVATE exemploNFe)

if(CPPBRASIL_EXAMPLES_WIDGETS)
    find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets)
    add_executable(widgetsNFe WIN32 widgets/main.cpp)
    target_link_libraries(widgetsNFe PRIVATE Qt6::Widgets exemploNFe)
endif()

# Copia o modelo de configuração para a pasta do executável.
configure_file(console/consoleNFe.ini.exemplo ${CMAKE_BINARY_DIR}/bin/consoleNFe.ini.exemplo COPYONLY)
