cmake_minimum_required(VERSION 3.14)

project(CppBrasil LANGUAGES CXX)

option(CPPBRASIL_BUILD_DANFE "Compila as bibliotecas CppDanfe e CppDanfeQtRPT" ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if (CMAKE_SYSTEM MATCHES "Linux")
        set(CMAKE_INSTALL_PREFIX
            "/usr" CACHE PATH "CppBrasil default install prefix" FORCE)
    endif()
endif()

include(GNUInstallDirs)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network)

add_subdirectory(CppBase)
add_subdirectory(CppNFe)

if(CPPBRASIL_BUILD_DANFE)
    add_subdirectory(CppDanfe)
    add_subdirectory(DanfeQtRPT)
endif()
