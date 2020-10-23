PROJECT(ScreenShot)

#SET(RABBITIM_PLUG_NAME CPluginScreenShot)

ADD_PLUGIN_TARGET(
    SOURCE_FILES
        PluginScreenShot.cpp
        DlgScreenShot.cpp
        WdgScreenEditor.cpp
    INCLUDE_DIRS $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Src>
    )

#include(${CMAKE_SOURCE_DIR}/Plugin/cmake/PluginApp.cmake)
