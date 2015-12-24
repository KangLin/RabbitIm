PROJECT(QRCode)

SET(RABBITIM_PLUG_NAME CPluginQRCode)

SET(PLUGIN_SOURCES
    CPluginQRCode.cpp 
    FrmScanQRcode.cpp    
    )

SET(PLUGIN_UIS
         FrmScanQRcode.ui
    )

include(${CMAKE_SOURCE_DIR}/cmake/PluginCommon.cmake)
