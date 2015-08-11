 QT += positioning quickwidgets

SOURCES += \
    $$PWD/LbsTrack.cpp \
    $$PWD/Nmea.cpp \
    $$PWD/LbsPositionLogger.cpp \
    $$PWD/LbsMotion.cpp \
    $$PWD/FrmLbsMain.cpp

HEADERS  += \
    $$PWD/LbsTrack.h \
    $$PWD/Nmea.h \
    $$PWD/LbsPositionLogger.h \
    $$PWD/LbsMotion.h \
    $$PWD/FrmLbsMain.h

FORMS    += \
    $$PWD/LbsTrack.ui \
    $$PWD/LbsMotion.ui \
    $$PWD/FrmLbsMain.ui

RESOURCES += \
    $$PWD/lbs.qrc

OTHER_FILES += \
    $$PWD/res/Map.qml 
