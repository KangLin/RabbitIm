 QT += positioning quickwidgets

SOURCES += \
    LbsTrack.cpp \
    Nmea.cpp \
    $$PWD/LbsPositionLogger.cpp

HEADERS  += \
    LbsTrack.h \
    Nmea.h \
    $$PWD/LbsPositionLogger.h

FORMS    += \
    LbsTrack.ui

RESOURCES += \
    $$PWD/lbs.qrc
