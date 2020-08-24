option(USE_OPENCV "Use opencv" ON)
if(NOT USE_OPENCV)
    return()
endif()

PROJECT(PluginConverFormatOpenCV)

find_package(OpenCV)
message("== Plug OpenCV:${OpenCV_FOUND}")
if(NOT OpenCV_FOUND)
    return()
endif()

ADD_PLUGIN_TARGET(
    PLUGIN_TYPE ConverFormat
    SOURCE_FILES ConverFormatOpenCV.cpp ConverFormatOpenCV.h
    INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/Src ${CMAKE_SOURCE_DIR}/Plugin
    )
