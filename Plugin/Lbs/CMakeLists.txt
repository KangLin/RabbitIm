PROJECT(Lbs)

SET(QT_COMPONENTS Positioning QuickWidgets)
FIND_PACKAGE(Qt5 COMPONENTS ${QT_COMPONENTS})
if(NOT Qt5_FOUND)
    return()
endif()
FOREACH(_COMPONENT ${QT_COMPONENTS})
    SET(RABBITIM_LIBS ${RABBITIM_LIBS} ${Qt5${_COMPONENT}_LIBRARIES})
ENDFOREACH()
SET(RABBITIM_PLUG_NAME CPluginAppMotion)

SET(QT_PLUGINS ${QT_PLUGINS} Qt5Positioning_PLUGINS Qt5QuickWidgets_PLUGINS)

SET(PLUGIN_SOURCES
    LbsTrack.cpp
    Nmea.cpp
    LbsPositionLogger.cpp
    LbsMotion.cpp
    FrmLbsMain.cpp
    LbsCamera.cpp
    PluginAppMotion.cpp
    )

SET(PLUGIN_UIS
    LbsTrack.ui
    LbsMotion.ui
    )

SET(PLUGIN_RESOURCE_FILES Resource.qrc)

IF(ANDROID)
    SET(PLUGIN_SOURCES
        ${PLUGIN_SOURCES}
        android/jni/CameraAndroid.cpp 
        android/jni/CameraAndroidResultReceiver.cpp
    )
ENDIF(ANDROID)

include(${CMAKE_SOURCE_DIR}/Plugin/cmake/PluginApp.cmake)
