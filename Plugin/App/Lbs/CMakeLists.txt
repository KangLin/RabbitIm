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
        Android/jni/CameraAndroid.cpp
        Android/jni/CameraAndroidResultReceiver.cpp
    )
ENDIF(ANDROID)

file(RELATIVE_PATH PLUGIN_RELATIVE_DIR ${CMAKE_SOURCE_DIR}/Plugin ${CMAKE_CURRENT_SOURCE_DIR})
if(NOT ANDROID)
    set(PLUGIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/plugins/${PLUGIN_RELATIVE_DIR})
    set(PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/plugins/${PLUGIN_RELATIVE_DIR})
endif()
ADD_PLUGIN_TARGET(
    SOURCE_FILES ${PLUGIN_SOURCES} Resource.qrc
    PRIVATE_LIBS RabbitIm ${QT_LIBRARIES}
    INCLUDE_DIRS $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Src>
    OUTPUT_DIR ${PLUGIN_OUTPUT_DIR}
    INSTALL_DIR ${PLUGIN_INSTALL_DIR}
    )

if(BUILD_PLUGIN_APP)
    ADD_TARGET(
        NAME ${PROJECT_NAME}App
        ISEXE
        SOURCE_FILES main.cpp
        PRIVATE_LIBS ${PROJECT_NAME} RabbitIm ${QT_LIBRARIES}
        INCLUDE_DIRS $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
        OUTPUT_DIR ${PLUGIN_OUTPUT_DIR}
        INSTALL_DIR "${CMAKE_INSTALL_BINDIR}")
endif()
