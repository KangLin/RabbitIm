PROJECT(Lbs)

SET(QT_COMPONENTS Positioning QuickWidgets)
FIND_PACKAGE(Qt5 COMPONENTS ${QT_COMPONENTS})
if(NOT Qt5_FOUND)
    return()
endif()
FOREACH(_COMPONENT ${QT_COMPONENTS})
    SET(RABBITIM_LIBS ${RABBITIM_LIBS} ${Qt5${_COMPONENT}_LIBRARIES})
ENDFOREACH()
#SET(RABBITIM_PLUG_NAME CPluginAppMotion)

#SET(QT_PLUGINS ${QT_PLUGINS} Qt5Positioning_PLUGINS Qt5QuickWidgets_PLUGINS)

set(PLUGIN_SOURCES
        LbsTrack.cpp
        Nmea.cpp
        LbsPositionLogger.cpp
        LbsMotion.cpp
        FrmLbsMain.cpp
        LbsCamera.cpp
        PluginAppMotion.cpp
        LbsTrack.ui
        LbsMotion.ui
   )

IF(ANDROID)
    SET(PLUGIN_SOURCES
        ${PLUGIN_SOURCES}
        android/jni/CameraAndroid.cpp 
        android/jni/CameraAndroidResultReceiver.cpp
    )
ENDIF(ANDROID)

ADD_PLUGIN_TARGET(
    SOURCE_FILES ${PLUGIN_SOURCES}
    RESOURCE_FILES Resource.qrc
    INCLUDE_DIRS $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Src>
    )

#include(${CMAKE_SOURCE_DIR}/Plugin/cmake/PluginApp.cmake)
