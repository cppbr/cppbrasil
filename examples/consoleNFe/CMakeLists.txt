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
        ../../third-party/windows/OpenSSL1.1.1q/include
        ../../third-party/windows/libxml2-v2.9.13/include/libxml2
        ../../third-party/windows/libiconv-for-Windows/include
        ../../third-party/windows/zlib-1.2.12/include
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
    target_link_libraries(consoleNFe  PRIVATE -lCppNFe -lcrypto -lssl -lxml2 -lz)
elseif (CMAKE_SYSTEM MATCHES "Windows")
    target_link_directories(consoleNFe
        PRIVATE ../../lib
        PRIVATE ../../third-party/windows/OpenSSL1.1.1q/lib
        PRIVATE ../../third-party/windows/libxml2-v2.9.13/lib
        PRIVATE ../../third-party/windows/libiconv-for-Windows/lib64
        PRIVATE ../../third-party/windows/zlib-1.2.12/lib
        PRIVATE ../../third-party/windows/QtRptProject3.0/lib/debug/lib
    )
    target_link_libraries(consoleNFe PRIVATE CppNFe libcrypto libssl libxml2 zdll)
endif()
