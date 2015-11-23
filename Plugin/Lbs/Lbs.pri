QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

CONFIG += c++0x
!msvc{
    QMAKE_CXXFLAGS += " -std=c++0x "
}

QT += positioning quickwidgets 

include(../config.pri)

SOURCES += \
    $$PWD/LbsTrack.cpp \
    $$PWD/Nmea.cpp \
    $$PWD/LbsPositionLogger.cpp \
    $$PWD/LbsMotion.cpp \
    $$PWD/FrmLbsMain.cpp \
    $$PWD/LbsCamera.cpp \
    $$PWD/PluginAppMotion.cpp

HEADERS  += \
    $$PWD/LbsTrack.h \
    $$PWD/Nmea.h \
    $$PWD/LbsPositionLogger.h \
    $$PWD/LbsMotion.h \
    $$PWD/FrmLbsMain.h \
    $$PWD/LbsCamera.h \
    $$PWD/PluginAppMotion.h

FORMS    += \
    $$PWD/LbsTrack.ui \
    $$PWD/LbsMotion.ui \
    $$PWD/FrmLbsMain.ui

RESOURCES += \
    $$PWD/lbs.qrc

DISTFILES += \
    $$PWD/CMakeLists.txt \
    $$PWD/res/Map.qml \
    $$PWD/res/Marker.qml 

android{
    include($$PWD/Android/Android.pri)
}

