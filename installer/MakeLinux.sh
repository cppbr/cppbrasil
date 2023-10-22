#!/bin/bash

CPPBRASIL_PATH="../.."
XML2_PATH="/usr/include/libxml2"
QTRPT_HEADER="../../third-party/linux/QtRptProject3.0/include/QtRPT"

echo "Informe o Diretorio do Qt onde se encontra o gcc_64: "
read  QT_GCC_PATH;

if [ -z $QT_GCC_PATH ]; then
    CMD_PREFIX_PATH=""
else
    CMD_PREFIX_PATH="-DCMAKE_PREFIX_PATH=$QT_GCC_PATH"
fi

echo
echo
echo "Caminho para instalacao. Caso nao informe o padrao sera: /usr/"
echo  "Informe o caminho para instalcao : "
read INSTALL_PATH;

if [ -z $INSTALL_PATH ]; then
    CMD_INSTALL=""
else
    CMD_INSTALL="-DCMAKE_INSTALL_PREFIX=$INSTALL_PATH"
fi

echo
echo
echo  "Informe o Tipo de Compilacao [D] para Debug [R] para Release:"
read  BUILD_TYPE;

if [ -z $BUILD_TYPE ] || [ $BUILD_TYPE  == "R" ] || [ $BUILD_TYPE  == "r" ]; then
    CMD_BUILD_TYPE="-DCMAKE_BUILD_TYPE=Release"
    QTRPT_LIB="../../third-party/linux/QtRptProject3.0/lib/release"
else
    CMD_BUILD_TYPE="-DCMAKE_BUILD_TYPE=Debug"
    QTRPT_LIB="../../third-party/linux/QtRptProject3.0/lib/debug"

fi


rm -fr build
mkdir build
cd build


#CPLUS_INCLUDE_PATH include para C++. Se for C é C_INCLUDE_PATH
CPLUS_INCLUDE_PATH=$XML2_PATH:$QTRPT_HEADER:$CPLUS_INCLUDE_PATH
export CPLUS_INCLUDE_PATH

CPPNFE=$CPPBRASIL_PATH"/CppBrasil/NFe"
cmake -GNinja $CPPNFE $CMD_BUILD_TYPE $CMD_PREFIX_PATH $CMD_INSTALL
#se a instalação for no /usr/ pede senha do sudo
if [ -z $CMD_INSTALL ]; then
    ninja && sudo ninja install
else
    ninja && ninja install
fi

rm -fr CMakeFiles CppNFe_autogen build.ninja CMakeCache.txt cmake_install.cmake install_manifest.txt

#CPPDANFEQTRPT------------------------------

#caso a instação seja em /usr/lib não haverá nenhuma flag para link das libs CppNFe e QtRPt, apenas copiará a libQtRPT para /usr/lib
if [ -z $CMD_INSTALL ]; then
    sudo cp -R $QTRPT_HEADER /usr/include
    CPLIB="$QTRPT_LIB/libQtRPT.so"
    sudo cp $CPLIB /usr/lib
    LINKER_FLAG=""
else
#caso informe outro caminho para instação, haverá as flags para link das libs CppNFe e QtRPt. Também copiará a libQtRPT para o diretório informado
    cp -R $QTRPT_HEADER "$INSTALL_PATH/include"
    CPLIB="$QTRPT_LIB/libQtRPT.so"
    cp $CPLIB "$INSTALL_PATH/lib"
    LINKER_FLAG="-DCMAKE_SHARED_LINKER_FLAGS=-L$INSTALL_PATH/lib  "
fi


CPPDANFEQTRPT=$CPPBRASIL_PATH"/CppBrasil/DanfeQtRPT"

cmake -GNinja $CPPDANFEQTRPT $CMD_BUILD_TYPE $CMD_PREFIX_PATH $LINKER_FLAG $CMD_INSTALL
if [ -z $CMD_INSTALL ]; then
    ninja && sudo ninja install
else
    ninja && ninja install
fi

cd ..

rm -fr build
