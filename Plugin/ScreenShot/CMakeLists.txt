PROJECT(ScreenShot)

SET(RABBITIM_PLUG_NAME CPluginScreenShot)

SET(PLUGIN_SOURCES
        PluginScreenShot.cpp
        DlgScreenShot.cpp
        WdgScreenEditor.cpp
    )

include(${CMAKE_SOURCE_DIR}/cmake/PluginCommon.cmake)
