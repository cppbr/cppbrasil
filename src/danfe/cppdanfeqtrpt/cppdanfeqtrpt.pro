QT    += core gui xml network charts qml sql

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
    ../barcode128.cpp \
    ../code128.cpp \
    ../cppdanfe.cpp \
    cppdanfeqtrpt.cpp \

HEADERS += \
    ../barcode128.h \
    ../code128.h \
    ../code128_const.h \
    ../cppdanfe.h \
    cppdanfeqtrpt.h \
    cppdanfeqtrpt_p.h



# Default rules for deployment.



unix {
target.path = /usr/lib
INCLUDEPATH += $$PWD/..

#cppnfe
INCLUDEPATH +=  /home/joao/cppbrasil/src \
                /home/joao/cppbrasil/src/cppnfe
LIBS += -L/home/joao/cppbrasil/debug/src/cppnfe -lcppnfe

#openssl, libxml2, zlib
INCLUDEPATH += /home/joao/Lib/openssl/include \
               /home/joao/Lib/libxml2/include \
               /home/joao/Lib/zlib-1.2.11/bin/include

LIBS += /home/joao/Lib/openssl/libcrypto.a -ldl -lpthread
LIBS += /home/joao/Lib/openssl/libssl.a
LIBS += -L/home/joao/Lib/libxml2/ -lxml2
LIBS += -L/home/joao/Lib/zlib-1.2.11/bin/lib/ -lz

#QtRpt
INCLUDEPATH += /home/joao/Lib/QtRptProject/QtRPT
LIBS += -L/home/joao/Lib/QtRptProject/bin/debug/lib -lQtRPT

}
!isEmpty(target.path): INSTALLS += target
