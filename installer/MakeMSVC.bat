
@echo off

echo.
echo.

@echo off
set /p QT_MSVC_PATH=Informe o caminho da pasta msvc2019_64 ou superior que se encontra na instalacao do Qt: 
set INSTALL_PATH=..\..
set CPPBRASIL_PATH=..\..\CppBrasil
set OPENSSL_HEADER=..\..\third-party\windows\OpenSSL3.1.3\include
set OPENSSL_LIB=..\..\third-party\windows\OpenSSL3.1.3\lib
set LIBXML2_HEADER=..\..\third-party\windows\libxml2-v2.11.5\include\libxml2
set LIBXML2_LIB=..\..\third-party\windows\libxml2-v2.11.5\lib
set LIBICONV_HEADER=..\..\third-party\windows\libiconv-for-Windows\include 
set LIBICONV_LIB=..\..\third-party\windows\libiconv-for-Windows\lib64\Release
set ZLIB_HEADER=..\..\third-party\windows\zlib-1.3\include
set ZLIB_LIB=..\..\third-party\windows\zlib-1.3\lib
set QTRPT_HEADER=..\..\third-party\windows\QtRptProject3.0\include\QtRPT
set QTRPT_LIB=..\..\third-party\windows\QtRptProject3.0\lib\debug\lib

echo.
echo.

echo Informe o caminho do arquivo vcvars64.bat que se encontra na instalacao do Visual Studio
echo Caso nao informe, o padrao sera: C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build 
set /p PATH_VCVARS64=Informe o caminho do vcvars64.bat: 
echo.

if [%PATH_VCVARS64%]==[]  set PATH_VCVARS64=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build

set FILE_VCVARS64=%PATH_VCVARS64%\vcvars64.bat

call "%FILE_VCVARS64%"


mkdir build
cd build

set INCLUDE_CMD= /I %OPENSSL_HEADER% /I %LIBXML2_HEADER% /I %LIBICONV_HEADER% /I %ZLIB_HEADER% /I %QTRPT_HEADER%
set LIB_CMD= /LIBPATH:%OPENSSL_LIB% /LIBPATH:%LIBXML2_LIB% /LIBPATH:%LIBICONV_LIB% /LIBPATH:%ZLIB_LIB% /LIBPATH:%QTRPT_LIB%
cmake -GNinja %CPPBRASIL_PATH%\NFe -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=%QT_MSVC_PATH% -DCMAKE_CXX_FLAGS=" %INCLUDE_CMD% " -DCMAKE_SHARED_LINKER_FLAGS=" %LIB_CMD% " -DCMAKE_INSTALL_PREFIX=%INSTALL_PATH%
ninja
ninja install

rd /s /q CMakeFiles CppNFe_autogen 
del build.ninja CMakeCache.txt cmake_install.cmake install_manifest.txt .ninja_deps .ninja_log

cmake -GNinja %CPPBRASIL_PATH%\DanfeQtRPT -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=%QT_MSVC_PATH% -DCMAKE_CXX_FLAGS=" %INCLUDE_CMD% " -DCMAKE_SHARED_LINKER_FLAGS=" %LIB_CMD% " -DCMAKE_INSTALL_PREFIX=%INSTALL_PATH%
ninja
ninja install


cd ..

rd /s /q build