cmake_minimum_required(VERSION 3.5)

project(WidgetsDANFE VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Widgets Gui Xml Charts Qml Sql Svg PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Widgets Gui Xml Charts Qml Sql Svg PrintSupport)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
if (CMAKE_SYSTEM MATCHES "Linux")
    include_directories(
        /usr/include/libxml2
        /usr/include/QtRPT
    )
elseif (CMAKE_SYSTEM MATCHES "Windows")
    include_directories(
        ../../include
        ../../third-party/windows/OpenSSL1.1.1q/include
        ../../third-party/windows/libxml2-v2.9.13/include/libxml2
        ../../third-party/windows/libiconv-for-Windows/include
        ../../third-party/windows/zlib-1.2.12/include
        ../../third-party/windows/QtRptProject3.0/include/QtRPT
    )
endif()

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ../nota.h
        ../nota.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(WidgetsDANFE
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET WidgetsDANFE APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(WidgetsDANFE SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(WidgetsDANFE
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(WidgetsDANFE PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Svg
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

set_target_properties(WidgetsDANFE PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(WidgetsDANFE)
endif()

if (CMAKE_SYSTEM MATCHES "Linux")
    target_link_libraries(WidgetsDANFE  PRIVATE -lCppNFe -lQtRPT -lCppDanfeQtRPT -lcrypto -lssl -lxml2 -lz)
elseif (CMAKE_SYSTEM MATCHES "Windows")
    target_link_directories(WidgetsDANFE
        PRIVATE ../../lib
        PRIVATE ../../third-party/windows/OpenSSL1.1.1q/lib
        PRIVATE ../../third-party/windows/libxml2-v2.9.13/lib
        PRIVATE ../../third-party/windows/libiconv-for-Windows/lib64
        PRIVATE ../../third-party/windows/zlib-1.2.12/lib
        PRIVATE ../../third-party/windows/QtRptProject3.0/lib/debug/lib
    )
    target_link_libraries(WidgetsDANFE  PRIVATE CppNFe QtRPT CppDanfeQtRPT libcrypto libssl libiconv libxml2 zdll)

endif()

