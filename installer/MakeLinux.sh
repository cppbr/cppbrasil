#!/bin/bash

CPPBRASIL_PATH="../.."

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
else
    CMD_BUILD_TYPE="-DCMAKE_BUILD_TYPE=Debug"
fi

echo
echo
echo "Informe o caminho completo do diretorio com o qtrpt.h. Caso nao informe, o DANFE nao sera compilado: "
read QTRPT_HEADER;

if [ -z "$QTRPT_HEADER" ]; then
    CMD_DANFE="-DCPPBRASIL_BUILD_DANFE=OFF"
else
    echo "Informe o caminho completo do arquivo libQtRPT.so: "
    read QTRPT_LIB;
    CMD_DANFE="-DQTRPT_INCLUDE_DIR=$QTRPT_HEADER -DQTRPT_LIBRARY=$QTRPT_LIB"
fi


rm -fr build
mkdir build
cd build

cmake -GNinja $CPPBRASIL_PATH $CMD_BUILD_TYPE $CMD_PREFIX_PATH $CMD_INSTALL $CMD_DANFE
#se a instalação for no /usr/ pede senha do sudo
if [ -z $CMD_INSTALL ]; then
    ninja && sudo ninja install
else
    ninja && ninja install
fi

cd ..

rm -fr build
