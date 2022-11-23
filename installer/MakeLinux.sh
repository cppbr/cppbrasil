#!/bin/bash
CPPBRASIL_PATH="../.."
INSTALL_PATH=""
QTRPT_HEADER="../../third-party/linux/QtRptProject3.0/include/QtRPT"
QTRPT_LIB="../../third-party/linux/QtRptProject3.0/lib/debug"
XML2_PATH="/usr/include/libxml2"

rm -fr build
mkdir build


if [ -z $INSTALL_PATH ]; then
    CMD_INSTALL=""
else
    CMD_INSTALL="-DCMAKE_INSTALL_PREFIX=$INSTALL_PATH"
fi


cd build

echo "Informe o Diretorio do Qt onde se encontra o gcc_64: "
read QT_GCC_PATH

if [ -z $QT_GCC_PATH ]; then
    CMD_PREFIX_PATH=""
else
    CMD_PREFIX_PATH="-DCMAKE_PREFIX_PATH=$QT_GCC_PATH"
fi


#CPLUS_INCLUDE_PATH include para C++. Se for C é C_INCLUDE_PATH
CPLUS_INCLUDE_PATH=$XML2_PATH:$QTRPT_HEADER:$CPLUS_INCLUDE_PATH
export CPLUS_INCLUDE_PATH

CPPNFE=$CPPBRASIL_PATH"/CppBrasil/NFe"
cmake -GNinja $CPPNFE $CMD_PREFIX_PATH $CMD_INSTALL
ninja && sudo ninja install

rm -fr CMakeFiles CppNFe_autogen build.ninja CMakeCache.txt cmake_install.cmake install_manifest.txt

sudo cp -R $QTRPT_HEADER /usr/include
CPLIB="$QTRPT_LIB/libQtRPT.so"
sudo cp $CPLIB /usr/lib

echo $CXX_FLAGS
CPPDANFEQTRPT=$CPPBRASIL_PATH"/CppBrasil/DanfeQtRPT"

cmake -GNinja $CPPDANFEQTRPT $CMD_PREFIX_PATH $CMD_INSTALL
ninja && sudo ninja install

cd ..

rm -fr build
