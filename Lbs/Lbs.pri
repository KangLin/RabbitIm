 QT += positioning quickwidgets

SOURCES += \
    LbsTrack.cpp \
    Nmea.cpp \
    LbsPositionLogger.cpp

HEADERS  += \
    LbsTrack.h \
    Nmea.h \
    LbsPositionLogger.h

FORMS    += \
    LbsTrack.ui

RESOURCES += \
    lbs.qrc

OTHER_FILES += \
    res/Map.qml 
