
@echo off

echo.
echo.
@echo off
set /p QT_MSVC_PATH=Informe o caminho da pasta msvc2019_64 ou superior que se encontra na instalacao do Qt: 

echo.
set /p BUILD_TYPE=Informe o Tipo de Compilacao [D] para Debug [R] para Release:
if [%BUILD_TYPE%]==[] set BUILD_TYPE=Release
if [%BUILD_TYPE%]==[R] set BUILD_TYPE=Release
if [%BUILD_TYPE%]==[r] set BUILD_TYPE=Release
if [%BUILD_TYPE%]==[D] set BUILD_TYPE=Debug
if [%BUILD_TYPE%]==[d] set BUILD_TYPE=Debug

set INSTALL_PATH=..\..\build\%BUILD_TYPE%
set CPPBRASIL_PATH=..\..
echo.
echo Informe o caminho completo das dependencias. Cada pasta deve conter include e lib.
set /p OPENSSL_PATH=Pasta do OpenSSL: 
set /p LIBXML2_PATH=Pasta da libxml2: 
set /p LIBICONV_PATH=Pasta da libiconv: 
set /p ZLIB_PATH=Pasta da zlib: 

set OPENSSL_HEADER=%OPENSSL_PATH%\include
set OPENSSL_LIB=%OPENSSL_PATH%\lib
set LIBXML2_HEADER=%LIBXML2_PATH%\include\libxml2
set LIBXML2_LIB=%LIBXML2_PATH%\lib
set LIBICONV_HEADER=%LIBICONV_PATH%\include
set LIBICONV_LIB=%LIBICONV_PATH%\lib
set ZLIB_HEADER=%ZLIB_PATH%\include
set ZLIB_LIB=%ZLIB_PATH%\lib

echo.
set /p QTRPT_HEADER=Pasta com o qtrpt.h. Caso nao informe, o DANFE nao sera compilado: 
set CMD_DANFE=-DCPPBRASIL_BUILD_DANFE=OFF
if "%QTRPT_HEADER%"=="" goto fim_qtrpt
set /p QTRPT_LIBRARY=Caminho completo do arquivo QtRPT.lib: 
set CMD_DANFE=-DQTRPT_INCLUDE_DIR=%QTRPT_HEADER% -DQTRPT_LIBRARY=%QTRPT_LIBRARY%
:fim_qtrpt

echo.
echo.
echo Informe o caminho do arquivo vcvars64.bat que se encontra na instalacao do Visual Studio
echo Caso nao informe, o padrao sera: C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build 
set /p PATH_VCVARS64=Informe o caminho do vcvars64.bat: 
echo.

if [%PATH_VCVARS64%]==[] set PATH_VCVARS64=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build

set FILE_VCVARS64=%PATH_VCVARS64%\vcvars64.bat

call "%FILE_VCVARS64%"


mkdir build
cd build

set INCLUDE_CMD= /I %OPENSSL_HEADER% /I %LIBXML2_HEADER% /I %LIBICONV_HEADER% /I %ZLIB_HEADER%
set LIB_CMD= /LIBPATH:%OPENSSL_LIB% /LIBPATH:%LIBXML2_LIB% /LIBPATH:%LIBICONV_LIB% /LIBPATH:%ZLIB_LIB%
cmake -GNinja %CPPBRASIL_PATH% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_PREFIX_PATH=%QT_MSVC_PATH% -DCMAKE_CXX_FLAGS=" %INCLUDE_CMD% " -DCMAKE_SHARED_LINKER_FLAGS=" %LIB_CMD% " -DCMAKE_INSTALL_PREFIX=%INSTALL_PATH% %CMD_DANFE%
ninja
ninja install

cd ..

rd /s /q build
