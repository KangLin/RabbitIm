SET(QT_COMPONENTS Positioning QuickWidgets)
FIND_PACKAGE(Qt5 CONFIG REQUIRED ${QT_COMPONENTS})
FOREACH(_COMPONENT ${QT_COMPONENTS})
    FIND_PACKAGE(Qt5${_COMPONENT} REQUIRED)
    SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5${_COMPONENT}_LIBRARIES})
ENDFOREACH()


SET(QT_PLUGINS ${QT_PLUGINS} Qt5Positioning_PLUGINS Qt5QuickWidgets_PLUGINS)

SET(LBS_DIR ${CMAKE_SOURCE_DIR}/Plugin/Lbs)

SET(RABBITIM_RCC_FILES ${RABBITIM_RCC_FILES} ${LBS_DIR}/lbs.qrc)

SET(RABBITIM_SOURCES
    ${RABBITIM_SOURCES}
    ${LBS_DIR}/LbsTrack.cpp 
    ${LBS_DIR}/Nmea.cpp 
    ${LBS_DIR}/LbsPositionLogger.cpp 
    ${LBS_DIR}/LbsMotion.cpp 
    ${LBS_DIR}/FrmLbsMain.cpp 
    ${LBS_DIR}/LbsCamera.cpp 
    ${LBS_DIR}/PluginAppMotion.cpp
)

IF(ANDROID)
    SET(RABBITIM_SOURCES
        ${RABBITIM_SOURCES}
        ${LBS_DIR}/android/jni/CameraAndroid.cpp 
        ${LBS_DIR}/android/jni/CameraAndroidResultReceiver.cpp
    )
ENDIF(ANDROID)
