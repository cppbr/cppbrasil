cmake_minimum_required(VERSION 3.16)

project(QuickNFe VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

if(CMAKE_SYSTEM MATCHES Android)
    include_directories(/media/Secundario/AndroidSDK/android_openssl/static/include)
    include_directories(/home/joao/Lib/libxml2-v2.9.13/out/include/libxml2)
    include_directories(/home/joao/Lib/zlib-1.2.11/build-clang/include)
    include_directories(/home/joao/cppbrasil)
elseif(CMAKE_SYSTEM MATCHES Linux)
 #   include_directories(/home/joao/cppbrasil)
    include_directories(/usr/include/libxml2)
endif()


qt_add_executable(appQuickNFe
    main.cpp
    ../nota.h
    ../nota.cpp
    android/AndroidManifest.xml
    #${CPPNFE}
)

qt_add_qml_module(appQuickNFe
    URI QuickNFe
    VERSION 1.0
    QML_FILES main.qml
    RESOURCES images/nfe.png
)

set_target_properties(appQuickNFe PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER cppbr.QuickNFe.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
target_link_libraries(appQuickNFe PRIVATE
    Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(appQuickNFe PRIVATE
    Qt6::Network)

#manifest file
set_target_properties(appQuickNFe PROPERTIES
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
)

target_compile_definitions(appQuickNFe
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)


if(CMAKE_SYSTEM MATCHES Android)
    target_link_libraries(appQuickNFe PRIVATE /media/Secundario/AndroidSDK/android_openssl/latest/x86_64/libcrypto_1_1.so)
    target_link_libraries(appQuickNFe PRIVATE /media/Secundario/AndroidSDK/android_openssl/latest/x86_64/libssl_1_1.so)
    target_link_libraries(appQuickNFe PRIVATE /home/joao/Lib/libxml2-v2.9.13/out/lib/libxml2.so)
    target_link_libraries(appQuickNFe PRIVATE /media/Secundario/AndroidSDK/ndk/22.1.7171670/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/x86_64-linux-android/30/libz.so)
    target_link_libraries(appQuickNFe PRIVATE /home/joao/QtCon/exemplos/lib/x86_64/libCppNFe.so)
elseif(CMAKE_SYSTEM MATCHES Linux)
#    target_link_directories(appQuickNFe PRIVATE "/home/joao/cppbrasil/bin/lib/debug")
    target_link_libraries(appQuickNFe  PRIVATE -lCppNFe -lcrypto -lssl -lxml2 -lz )
endif()

