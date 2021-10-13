PROJECT(Lbs)

SET(QT_COMPONENTS Positioning QuickWidgets)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS})
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        list(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
else()
    return()
endif()
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
    SOURCE_FILES ${PLUGIN_SOURCES} Resource.qrc
    PRIVATE_LIBS RabbitIm ${QT_LIBRARIES}
    INCLUDE_DIRS $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Src>
    )

#include(${CMAKE_SOURCE_DIR}/Plugin/cmake/PluginApp.cmake)
