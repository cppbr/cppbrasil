#!/bin/bash

CPPBRASIL_PATH="../.."
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

cmake -GNinja $CPPBRASIL_PATH $CMD_BUILD_TYPE $CMD_PREFIX_PATH $CMD_INSTALL
#se a instalação for no /usr/ pede senha do sudo
if [ -z $CMD_INSTALL ]; then
    ninja && sudo ninja install
else
    ninja && ninja install
fi

#copia a libQtRPT e seus headers para o mesmo destino das libs da CppBrasil
if [ -z $CMD_INSTALL ]; then
    sudo cp -R $QTRPT_HEADER /usr/include
    sudo cp "$QTRPT_LIB/libQtRPT.so" /usr/lib
else
    cp -R $QTRPT_HEADER "$INSTALL_PATH/include"
    cp "$QTRPT_LIB/libQtRPT.so" "$INSTALL_PATH/lib"
fi

cd ..

rm -fr build
