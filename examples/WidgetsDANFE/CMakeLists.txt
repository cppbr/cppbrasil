cmake_minimum_required(VERSION 3.5)

project(WidgetsDANFE VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network Widgets Gui PrintSupport)

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

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ../nota.h
        ../nota.cpp
)

qt_add_executable(WidgetsDANFE
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET WidgetsDANFE APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

target_link_libraries(WidgetsDANFE PRIVATE
    Qt6::Widgets
    Qt6::Network
    Qt6::Gui
    Qt6::PrintSupport
)

set_target_properties(WidgetsDANFE PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(WidgetsDANFE)

if (CMAKE_SYSTEM MATCHES "Linux")
    target_link_libraries(WidgetsDANFE  PRIVATE -lCppDanfeBrReport -lCppDanfe -lCppNFe -lCppBase -lcrypto -lssl -lxml2 -lz)
elseif (CMAKE_SYSTEM MATCHES "Windows")
    target_link_directories(WidgetsDANFE
        PRIVATE ../../lib
    )
    target_link_libraries(WidgetsDANFE  PRIVATE CppDanfeBrReport CppDanfe CppNFe CppBase libcrypto libssl libiconv libxml2 zdll)

endif()

