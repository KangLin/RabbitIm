QT       += core gui widgets multimedia

CONFIG += c++0x
!msvc{
    QMAKE_CXXFLAGS += " -std=c++0x "
}

include(../config.pri)

SOURCES += CPluginQRCode.cpp \
    ../PluginApp.cpp  \
    FrmScanQRcode.cpp

HEADERS += CPluginQRCode.h \
    FrmScanQRcode.h

FORMS += FrmScanQRcode.ui

RESOURCES += \
    ../../Resource/Resource.qrc 

DISTFILES += QRCode.json

INCLUDEPATH += $$PWD/../..

INSTALLS += target
