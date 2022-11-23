@echo off

@echo off
set /p QT_MSVC_PATH=Informe o Diretorio do Qt onde se encontra o msvc2019_64 ou superior: 
set INSTALL_PATH=..\..
set CPPBRASIL_PATH=..\..\CppBrasil
set OPENSSL_HEADER=..\..\third-party\windows\OpenSSL1.1.1q\include
set OPENSSL_LIB=..\..\third-party\windows\OpenSSL1.1.1q\lib
set LIBXML2_HEADER=..\..\third-party\windows\libxml2-v2.9.13\include\libxml2
set LIBXML2_LIB=..\..\third-party\windows\libxml2-v2.9.13\lib
set LIBICONV_HEADER=..\..\third-party\windows\libiconv-for-Windows\include 
set LIBICONV_LIB=..\..\third-party\windows\libiconv-for-Windows\lib64
set ZLIB_HEADER=..\..\third-party\windows\zlib-1.2.12\include
set ZLIB_LIB=..\..\third-party\windows\zlib-1.2.12\lib
set QTRPT_HEADER=..\..\third-party\windows\QtRptProject3.0\include\QtRPT
set QTRPT_LIB=..\..\third-party\windows\QtRptProject3.0\lib\debug\lib

call vcvars64.bat


mkdir build
cd build

set INCLUDE_CMD= /I %OPENSSL_HEADER% /I %LIBXML2_HEADER% /I %LIBICONV_HEADER% /I %ZLIB_HEADER% /I %QTRPT_HEADER%
set LIB_CMD= /LIBPATH:%OPENSSL_LIB% /LIBPATH:%LIBXML2_LIB% /LIBPATH:%LIBICONV_LIB% /LIBPATH:%ZLIB_LIB% /LIBPATH:%QTRPT_LIB%
cmake -GNinja %CPPBRASIL_PATH%\NFe -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=%QT_MSVC_PATH% -DCMAKE_CXX_FLAGS=" %INCLUDE_CMD% " -DCMAKE_SHARED_LINKER_FLAGS=" %LIB_CMD% " -DCMAKE_INSTALL_PREFIX=%INSTALL_PATH%
ninja
ninja install

rd /s /q CMakeFiles CppNFe_autogen 
del build.ninja CMakeCache.txt cmake_install.cmake install_manifest.txt .ninja_deps .ninja_log

cmake -GNinja %CPPBRASIL_PATH%\DanfeQtRPT -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=%QT_MSVC_PATH% -DCMAKE_CXX_FLAGS=" %INCLUDE_CMD% " -DCMAKE_SHARED_LINKER_FLAGS=" %LIB_CMD% " -DCMAKE_INSTALL_PREFIX=%INSTALL_PATH%
ninja
ninja install


cd ..

rd /s /q build
