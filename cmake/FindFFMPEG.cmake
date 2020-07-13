# - Try to find FFMPEG
# Para: 
#   FFMPEG_DIR
#   USE_PKG_CONFIG: Use pkg-config find ffmpeg
# Once done this will define
#   FFMPEG_FOUND
#   FFMPEG_INCLUDE_DIRS
#   FFMPEG_LIBRARIES
#   FFMPEG_DEFINITIONS
#
# Author: Kang Lin <kl222@126.com>

include(FindPackageHandleStandardArgs)

if (NOT FFMPEG_FIND_COMPONENTS)
    set(FFMPEG_FIND_COMPONENTS avcodec avdevice avfilter avformat avutil swscale)
endif(NOT FFMPEG_FIND_COMPONENTS)

# Generate component include files and requirements
foreach(comp ${FFMPEG_FIND_COMPONENTS})
    if(FFMPEG_FIND_REQUIRED_${comp})
	list(APPEND required "FFMPEG_${comp}_FOUND")
    endif()
endforeach(comp)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(_lib_suffix 64)
else()
    set(_lib_suffix 32)
endif()

# Find libraries
find_package(PkgConfig QUIET)
foreach(comp ${FFMPEG_FIND_COMPONENTS})
    if(PKG_CONFIG_FOUND AND USE_PKG_CONFIG)
        pkg_check_modules(_${comp} lib${comp})
        SET(FFMPEG_${comp}_LIBRARY ${_${comp}_LIBRARIES} ${_${comp}_LIBRARY_DIRS})
        SET(FFMPEG_${comp}_INCLUDE_DIR ${_${comp}_INCLUDE_DIRS})
    endif()
    if(NOT _${comp}_INCLUDE_DIRS)
        find_path(FFMPEG_${comp}_INCLUDE_DIR
            NAMES "lib${comp}/${comp}.h"
            HINTS
            ${FFMPEG_DIR}
            ENV FFMPEG_DIR
            PATHS
            /usr /usr/local /opt/local /sw
            PATH_SUFFIXES FFMPEG libav include lib${comp} include/lib${comp}
            DOC "FFMPEG include directory")
    else()
        SET(FFMPEG_${comp}_INCLUDE_DIR ${_${comp}_INCLUDE_DIRS})
    endif()
    
    if(NOT _${comp}_LIBRARIES)
        find_library(FFMPEG_${comp}_LIBRARY
            NAMES ${comp} ${comp}-FFMPEG ${_${comp}_LIBRARIES}
            HINTS
            ${FFMPEG_DIR}
            ENV FFMPEG_DIR
            PATHS
            /usr /usr/local /opt/local /sw
            PATH_SUFFIXES lib ${comp} lib${comp}
            DOC "FFMPEG ${comp} library")
    else()
        SET(FFMPEG_${comp}_LIBRARY ${_${comp}_LIBRARIES})
    endif()
    message("${comp}_LIBRARIES:${FFMPEG_${comp}_LIBRARY}")
    find_package_handle_standard_args(FFMPEG_${comp}
        FOUND_VAR FFMPEG_${comp}_FOUND
        REQUIRED_VARS FFMPEG_${comp}_LIBRARY FFMPEG_${comp}_INCLUDE_DIR)
    if(${FFMPEG_${comp}_FOUND})
        list(APPEND FFMPEG_INCLUDE_DIRS ${FFMPEG_${comp}_INCLUDE_DIR})
        list(APPEND FFMPEG_LIBRARIES ${FFMPEG_${comp}_LIBRARY})
    endif()
endforeach(comp)

# Run checks via find_package_handle_standard_args
find_package_handle_standard_args(FFMPEG
	FOUND_VAR FFMPEG_FOUND
	REQUIRED_VARS ${required} FFMPEG_INCLUDE_DIRS FFMPEG_LIBRARIES)

if(FFMPEG_FOUND)
    set(FFMPEG_DEFINITIONS HAVE_FFMPEG)
endif()

mark_as_advanced(FFMPEG_DIR)
mark_as_advanced(USE_PKG_CONFIG)
