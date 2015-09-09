SET(PLUGIN_DIR ${CMAKE_SOURCE_DIR}/Plugin)

SET(RABBITIM_SOURCES
    ${RABBITIM_SOURCES}
    ${PLUGIN_DIR}/PluginApp.cpp 
    ${PLUGIN_DIR}/ManagePlugin.cpp 
)

include(${PLUGIN_DIR}/Lbs/CMakeLists.txt)
