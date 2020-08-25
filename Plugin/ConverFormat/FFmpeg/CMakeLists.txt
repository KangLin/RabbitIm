if(ANDROID)
    option(USE_FFMPEG "Use ffmpeg" OFF)
else()
    option(USE_FFMPEG "Use ffmpeg" ON)
endif()
message(STATUS "== Use ffmpeg: ${USE_FFMPEG}")
if(NOT USE_FFMPEG)
    return()
endif()

find_package(FFMPEG)
if(NOT FFMPEG_FOUND)
    return()
endif()

message("== FFMPGE:${FFMPEG_FOUND}")

project(PluginConverFormatFFMpeg)

ADD_PLUGIN_TARGET(
    PLUGIN_TYPE ConverFormat
    SOURCE_FILES ConverFormatFFmpeg.cpp ConverFormatFFmpeg.h
    INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/Src ${CMAKE_SOURCE_DIR}/Plugin
    #PRIVATE_LIBS ${FFMPEG_LIBRARIES}    
    )
