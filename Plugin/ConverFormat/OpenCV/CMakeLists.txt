option(USE_OPENCV "Use opencv" ON)
if(NOT USE_OPENCV)
    return()
endif()

PROJECT(PluginConverFormatOpenCV)

find_package(OpenCV COMPONENTS core imgproc video) # videoio)
IF(OpenCV_FOUND)
    list(APPEND RABBITIM_DEFINES RABBITIM_USE_OPENCV)
    LIST(APPEND RABBITIM_LIBS ${OpenCV_LIBS})
else()
    return()
ENDIF()
message("== Plug OpenCV:${OpenCV_FOUND}")


ADD_PLUGIN_TARGET(
    PLUGIN_TYPE ConverFormat
    SOURCE_FILES ConverFormatOpenCV.cpp ConverFormatOpenCV.h
    INCLUDE_DIRS $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Plugin>
    PRIVATE_DEFINITIONS ${RABBITIM_DEFINES} OpenCV_VERSION_MAJOR=${OpenCV_VERSION_MAJOR}
    PRIVATE_LIBS RabbitIm ${OpenCV_LIBS}
    )
