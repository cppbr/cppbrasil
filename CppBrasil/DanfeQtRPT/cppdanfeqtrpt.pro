QT    += core gui xml network charts qml sql svg

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets printsupport
    DEFINES += HAVE_QT5
}

greaterThan(QT_MAJOR_VERSION, 5):
    greaterThan(QT_MINOR_VERSION, 7):QT += charts

TEMPLATE = lib
DEFINES += CPPDANFEQTRPT_LIBRARY

linux{
    CONFIG += plugin
}

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../code128.cpp \
    ../cppbarcode.cpp \
    ../cppdanfe.cpp \
    ../qrcodegen.cpp \
    cppdanfeqtrpt.cpp \

HEADERS += \
    ../code128.h \
    ../cppbarcode.h \
    ../cppdanfe.h \
    ../qrcodegen.hpp \
    cppdanfeqtrpt.h \
    cppdanfeqtrpt_p.h



# Default rules for deployment.



unix {
target.path = /usr/lib
INCLUDEPATH += $$PWD/..

#cppnfe
INCLUDEPATH +=  /home/joao/cppbrasil/src \
                /home/joao/cppbrasil/src/cppnfe
LIBS += -L/home/joao/cppbrasil/debug/src/cppnfe -lCppNFe

#openssl, libxml2, zlib
INCLUDEPATH +=  /usr/include/libxml2
LIBS += -lcrypto -lssl  -lxml2 -lz

#QtRpt
INCLUDEPATH += /home/joao/Lib/QtRptProject/QtRPT
LIBS += -L/home/joao/Lib/QtRptProject/bin/debug/lib -lQtRPT

}
!isEmpty(target.path): INSTALLS += target
