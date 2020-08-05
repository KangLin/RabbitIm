cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(RabbitIm)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

if(POLICY CMP0083)
    cmake_policy(SET CMP0083 NEW)
endif()

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()

if(POLICY CMP0021)
    cmake_policy(SET CMP0021 NEW)
endif()

# 建立要删除的文件
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
# 建立删除目标
ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")

SET(BUILD_VERSION "v0.1.0")
# Find Git Version Patch
IF(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    if(NOT GIT)
        SET(GIT $ENV{GIT})
    endif()
    if(NOT GIT)
        FIND_PROGRAM(GIT NAMES git git.exe git.cmd)
    endif()
    IF(GIT)
        EXECUTE_PROCESS(
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND ${GIT} describe --tags
            OUTPUT_VARIABLE GIT_VERSION  OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if(NOT GIT_VERSION)
            EXECUTE_PROCESS(
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMAND ${GIT} rev-parse --short HEAD
                OUTPUT_VARIABLE GIT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE
            )
        endif()
        SET(BUILD_VERSION ${GIT_VERSION})
    ENDIF(GIT)
ENDIF()
message("BUILD_VERSION:${BUILD_VERSION}")

#CHECK_INCLUDE_FILE_CXX("string" HAVE_STRING_H)
#check_include_file("math.h" HAVE_MATH_H)

#check_function_exists("fabs" HAVE_FABS)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(NOT RabbitCommon_DIR)
    set(RabbitCommon_DIR $ENV{RabbitCommon_DIR})
    if(NOT RabbitCommon_DIR)
        set(RabbitCommon_DIR ${CMAKE_SOURCE_DIR}/../RabbitCommon)
    endif()
endif()
if(DEFINED RabbitCommon_DIR AND EXISTS ${RabbitCommon_DIR}/Src)
    include(${RabbitCommon_DIR}/cmake/RabbitCommonUtils.cmake)
    add_subdirectory(${RabbitCommon_DIR}/Src ${CMAKE_BINARY_DIR}/RabbitCommon)
else()
    message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon")
    message("   ag:")
    message("       git clone https://github.com/KangLin/RabbitCommon.git")
    message("2. Then set cmake value or environment variable RabbitCommon_DIR to download root dirctory.")
    message("   ag:")
    message(FATAL_ERROR "       cmake -DRabbitCommon_DIR= ")
endif()

include(cmake/Configure.cmake)
include(cmake/ThirdLibrary.cmake)

add_subdirectory(Src)
add_subdirectory(App)
add_subdirectory(Plugin)
IF(NOT BUILD_SHARED_LIBS)
    include(Plugin/PluginStatic.cmake)
ENDIF()

add_custom_target(install-runtime
  COMMAND
     "${CMAKE_COMMAND}" -DCMAKE_INSTALL_COMPONENT=Runtime 
         -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake"
)

#安装相关文件
set(OTHER_FILES
    License.md
    Authors.md
    ChangeLog.md
    )
if(ANDROID)
    INSTALL(FILES ${OTHER_FILES} DESTINATION "assets" COMPONENT Runtime)
else()
    INSTALL(FILES ${OTHER_FILES} DESTINATION "." COMPONENT Runtime)
endif()
