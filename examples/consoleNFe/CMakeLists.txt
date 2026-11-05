cmake_minimum_required(VERSION 3.14)

project(consoleNFe LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
if (CMAKE_SYSTEM MATCHES "Linux")
    include_directories(
        /usr/include/libxml2
    )
elseif (CMAKE_SYSTEM MATCHES "Windows")
    include_directories(
        ../../include
    )
endif()

add_executable(consoleNFe
  main.cpp
  ../nota.h
  ../nota.cpp
)

target_link_libraries(consoleNFe 
   PRIVATE Qt${QT_VERSION_MAJOR}::Core 
   PRIVATE Qt${QT_VERSION_MAJOR}::Network
)

if (CMAKE_SYSTEM MATCHES "Linux")
    target_link_libraries(consoleNFe  PRIVATE -lCppNFe -lCppBase -lcrypto -lssl -lxml2 -lz)
elseif (CMAKE_SYSTEM MATCHES "Windows")
    target_link_directories(consoleNFe
        PRIVATE ../../lib
    )
    target_link_libraries(consoleNFe PRIVATE CppNFe CppBase libcrypto libssl libiconv libxml2 zdll)
endif()
