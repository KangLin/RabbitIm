PROJECT(Update)

SET(RABBITIM_PLUG_NAME CPluginUpdate)

IF(RABBITIM_USE_LIBCURL)
    SET(PLUGIN_SOURCES
        PluginUpdate.cpp 
        DownLoad.cpp
        DownLoadHandle.cpp
        DlgUpdate.cpp
    )
    SET(PLUGIN_UIS
            DlgUpdate.ui
    )
ENDIF(RABBITIM_USE_LIBCURL)

include(${CMAKE_SOURCE_DIR}/cmake/PluginCommon.cmake)
