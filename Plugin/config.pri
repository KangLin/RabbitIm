CONFIG *= plugin 
android : CONFIG*=static

TARGET_PATH=$$OUT_PWD/../..
#设置目标输出目录  
win32{
    CONFIG(debug, debug|release) {
        TARGET_PATH=$${TARGET_PATH}/Debug
    } else {
        TARGET_PATH=$${TARGET_PATH}/Release
    }
}
LIBS += -L$${TARGET_PATH}   #包含 RabbitIm 库位置  
!exists("$$OUT_PWD") : mkpath($$OUT_PWD)
#message("TARGET_PATH:$${TARGET_PATH}")

SOURCES += ../PluginApp.cpp

include($$PWD/../pri/ThirdLibraryConfig.pri)
myPackagesExist(RabbitIm) : MYPKGCONFIG *= RabbitIm
include($$PWD/../pri/ThirdLibrary.pri)
include($$PWD/../pri/ThirdLibraryJoin.pri)

contains(TEMPLATE, lib){
    isEmpty(RABBITIM_PLUG_NAME) : message("Please set RABBITIM_PLUG_NAME to plug class name")
    FILE_NAME=$$PWD/PluginStatic.cpp
    PLUG_CONTENT = "Q_IMPORT_PLUGIN($${RABBITIM_PLUG_NAME})"
    FILE_CONTENT = $$cat($$FILE_NAME)
    !contains(FILE_CONTENT, $$PLUG_CONTENT){
        write_file($$FILE_NAME, PLUG_CONTENT, append)
    }

    FILE_NAME=$$PWD/PluginStatic.pri
    PLUG_CONTENT = "-l$${TARGET}"
    FILE_CONTENT = $$cat($$FILE_NAME) 
    !contains(FILE_CONTENT, $$PLUG_CONTENT){
        PLUG_CONTENT = "LIBS += -l$${TARGET}"
        write_file($$FILE_NAME, PLUG_CONTENT, append)
    }
} 
