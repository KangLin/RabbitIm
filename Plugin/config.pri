CONFIG *= plugin 

TARGET_PATH=$$OUT_PWD/../..
LIBS += -L$${TARGET_PATH}
!exists("$$OUT_PWD") : mkpath($$OUT_PWD)
#message("TARGET_PATH:$${TARGET_PATH}")

include($$PWD/../pri/ThirdLibraryConfig.pri)
include($$PWD/../pri/ThirdLibrary.pri)
MYPKGCONFIG *= RabbitIm
include($$PWD/../pri/ThirdLibraryJoin.pri)
