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

if(ANDROID)
    INSTALL(FILES ${FFMPEG_LIBRARIES}
        DESTINATION "libs/${ANDROID_ABI}"
            COMPONENT Runtime)
elseif(WIN32)
    INSTALL(FILES ${FFMPEG_LIBRARIES}
        DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime)
endif()

project(PluginConverFormatFFMpeg)

set(SOURCES_FILES
    ConverFormatFFmpeg.cpp
    )

set(HEADER_FILES
    ConverFormatFFmpeg.h
    )

set(SOURCE_UI_FILES
    )

# Install target
set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR})
if(NOT ANDROID)
    set(INSTALL_PATH ${INSTALL_FACE_RECOGNIZER_PLUGS_DIR}/ConverFormat)
endif()
ADD_PLUG_TARGET(NAME ${PROJECT_NAME}
    SOURCES_FILES ${SOURCES_FILES}
    HEADER_FILES ${HEADER_FILES}
    LIBS ${FFMPEG_LIBRARIES}
    INCLUDE_DIRS ${FFMPEG_INCLUDE_DIRS}
    DEFINITIONS ${FFMPEG_DEFINITIONS}
    OUTPUT_DIR ${FACE_RECOGNIZER_PLUGS_BINARY_DIR}/ConverFormat
    INSTALL_DIR ${INSTALL_PATH}
    VERSION ${BUILD_VERSION})
