#QZXing库
OPTION(OPTION_RABBITIM_USE_QZXING "Use QZXing library" ON)
if(NOT OPTION_RABBITIM_USE_QZXING)
    return()
endif()
FIND_PACKAGE(QZXing)
if(NOT QZXing_FOUND)
    return()
endif()

PROJECT(QRCode)

#SET(RABBITIM_PLUG_NAME CPluginQRCode)

ADD_PLUGIN_TARGET(
    SOURCE_FILES
        CPluginQRCode.cpp
        FrmScanQRcode.cpp
        FrmScanQRcode.ui
    INCLUDE_DIRS $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Src>
    )

#include(${CMAKE_SOURCE_DIR}/Plugin/cmake/PluginApp.cmake)
