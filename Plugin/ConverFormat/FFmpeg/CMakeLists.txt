if(ANDROID)
    option(USE_FFMPEG "Use ffmpeg" OFF)
else()
    option(USE_FFMPEG "Use ffmpeg" ON)
endif()
message(STATUS "== Use ffmpeg: ${USE_FFMPEG}")
if(NOT USE_FFMPEG)
    return()
endif()

find_package(FFMPEG COMPONENTS avcodec avformat avutil swscale)
if(FFMPEG_FOUND)
    list(APPEND RABBITIM_DEFINES __STDC_CONSTANT_MACROS RABBITIM_USE_FFMPEG)
else()
    return()
endif()
message("== FFMPGE:${FFMPEG_FOUND}")

project(PluginConverFormatFFMpeg)

file(RELATIVE_PATH PLUGIN_RELATIVE_DIR ${CMAKE_SOURCE_DIR}/Plugin ${CMAKE_CURRENT_SOURCE_DIR})
if(NOT ANDROID)
    set(PLUGIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/plugins/${PLUGIN_RELATIVE_DIR})
    set(PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/plugins/${PLUGIN_RELATIVE_DIR})
endif()
ADD_PLUGIN_TARGET(
    PLUGIN_TYPE ConverFormat
    SOURCE_FILES ConverFormatFFmpeg.cpp ConverFormatFFmpeg.h
    INCLUDE_DIRS $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Plugin>
    PRIVATE_LIBS RabbitIm ${FFMPEG_LIBRARIES}
    PRIVATE_DEFINITIONS ${RABBITIM_DEFINES}
    OUTPUT_DIR ${PLUGIN_OUTPUT_DIR}
    INSTALL_DIR ${PLUGIN_INSTALL_DIR}
    )
