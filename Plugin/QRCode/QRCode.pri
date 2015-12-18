QT *= core gui widgets multimedia

include(../ConfigApp.pri)

SOURCES += CPluginQRCode.cpp \
    FrmScanQRcode.cpp

HEADERS += CPluginQRCode.h \
    FrmScanQRcode.h

FORMS += FrmScanQRcode.ui

RESOURCES += \
    ../../Resource/Resource.qrc 

DISTFILES += QRCode.json
