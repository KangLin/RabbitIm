# - Try to find FFMPEG
# Once done this will define
#   FFMPEG_FOUND
#   FFMPEG_INCLUDE_DIRS
#   FFMPEG_LIBRARIES
#   FFMPEG_INCLUDE_FILES
# Author: Matt Coffin <mcoffin13@gmail.com>

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

set(FFMPEG_PATH_ARCH FFMPEG_PATH${_lib_suffix})
# Find libraries
find_package(PkgConfig QUIET)
foreach(comp ${FFMPEG_FIND_COMPONENTS})
	if(PKG_CONFIG_FOUND)
		pkg_check_modules(_${comp} QUIET lib${comp})
	endif()

	if(NOT _${comp}_INCLUDE_DIRS)
		find_path(FFMPEG_${comp}_INCLUDE_DIR
			NAMES "lib${comp}/${comp}.h"
			HINTS
				${_${comp}_INCLUDE_DIRS}
				ENV FFMPEG_PATH
				ENV ${FFMPEG_PATH_ARCH}
			PATHS
				/usr/include /usr/local/include /opt/local/include /sw/include
			PATH_SUFFIXES FFMPEG libav
			DOC "FFMPEG include directory")
	else()
		SET(FFMPEG_${comp}_INCLUDE_DIR ${_${comp}_INCLUDE_DIRS})
	endif()

	if(NOT _${comp}_LIBRARIES)
		find_library(FFMPEG_${comp}_LIBRARY
			NAMES ${comp} ${comp}-FFMPEG ${_${comp}_LIBRARIES}
			HINTS
				${_${comp}_LIBRARY_DIRS}
				"${FFMPEG_${comp}_INCLUDE_DIR}/../lib"
				"${FFMPEG_${comp}_INCLUDE_DIR}/../lib${_lib_suffix}"
				"${FFMPEG_${comp}_INCLUDE_DIR}/../libs${_lib_suffix}"
				"${FFMPEG_${comp}_INCLUDE_DIR}/lib"
				"${FFMPEG_${comp}_INCLUDE_DIR}/lib${_lib_suffix}"
			PATHS
				/usr/lib /usr/local/lib /opt/local/lib /sw/lib
			PATH_SUFFIXES ${comp} lib${comp}
			DOC "FFMPEG ${comp} library")
	else()
		SET(FFMPEG_${comp}_LIBRARY ${_${comp}_LIBRARIES})
	endif()
	message("_${comp}_LIBRARIES:${FFMPEG_${comp}_LIBRARY}")
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

