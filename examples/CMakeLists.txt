cmake_minimum_required(VERSION 3.21)

project(consoleNFe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# O exemplo compila a CppBrasil junto, a partir de src/. Para usar outra cópia
# dos fontes, informe -DCPPBRASIL_SRC=<pasta src da CppBrasil>.
set(CPPBRASIL_SRC "${CMAKE_CURRENT_LIST_DIR}/../src" CACHE PATH "Pasta src da CppBrasil")
set(CPPBRASIL_BUILD_DANFE OFF CACHE BOOL "" FORCE)
add_subdirectory(${CPPBRASIL_SRC} cppbrasil)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core)

add_executable(consoleNFe main.cpp)
target_link_libraries(consoleNFe PRIVATE Qt6::Core CppNFe)
# Executável na mesma pasta das DLLs da CppBrasil.
set_target_properties(consoleNFe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Copia o modelo de configuração para a pasta do executável.
configure_file(consoleNFe.ini.exemplo ${CMAKE_BINARY_DIR}/bin/consoleNFe.ini.exemplo COPYONLY)
