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
    INCLUDE_DIRS $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Plugin>
    PRIVATE_DEFINITIONS OpenCV_VERSION_MAJOR=${OpenCV_VERSION_MAJOR}
    #PRIVATE_LIBS ${OpenCV_LIBS}
    )
