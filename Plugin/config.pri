CONFIG *= plugin 

TARGET_PATH=$$OUT_PWD/../..
LIBS += -L$${TARGET_PATH}
!exists("$$OUT_PWD") : mkpath($$OUT_PWD)
#message("TARGET_PATH:$${TARGET_PATH}")

SOURCES += ../PluginApp.cpp

include($$PWD/../pri/ThirdLibraryConfig.pri)
MYPKGCONFIG *= RabbitIm
include($$PWD/../pri/ThirdLibrary.pri)
include($$PWD/../pri/ThirdLibraryJoin.pri)
