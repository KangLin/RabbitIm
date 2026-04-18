# Author: Kang Lin(kl222@126.com)

cmake_minimum_required(VERSION 3.21)
MESSAGE(STATUS "Found CMake ${CMAKE_VERSION}")

project(RabbitIm
    HOMEPAGE_URL "https://github.com/KangLin/RabbitIm"
    )

if(CMAKE_VERSION VERSION_LESS "3.7.0")
   set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 避免使用编译器特定的扩展语法，从而提高跨平台一致性。
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
# 确保编译器支持 C17
set(CMAKE_C_STANDARD_REQUIRED ON)
# 避免使用编译器特定的扩展语法，从而提高跨平台一致性。
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "verbose")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

if(POLICY CMP0083)
    cmake_policy(SET CMP0083 NEW)
endif()

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()

if(POLICY CMP0021)
    cmake_policy(SET CMP0021 NEW)
endif()

#需要的QT组件
SET(QT_COMPONENTS Core Gui Widgets)
if(ANDROID)
    SET(QT_COMPONENTS ${QT_COMPONENTS} AndroidExtras)
endif()
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS})
message("QT_VERSION:${Qt${QT_VERSION_MAJOR}_VERSION}")
if(Qt${QT_VERSION_MAJOR}_VERSION VERSION_LESS 5.10.0 AND ANDROID)
    message(FATAL_ERROR "Qt must great 5.10.0")
endif()
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        list(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
endif()
get_filename_component(QT_INSTALL_DIR "${Qt${QT_VERSION_MAJOR}_DIR}/../../.." ABSOLUTE)
message("QT_INSTALL_DIR:${QT_INSTALL_DIR}")
message("QT_LIBRARIES:${QT_LIBRARIES}")

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libs")
if (BUILD_SHARED_LIBS)
    add_definitions(-DBUILD_SHARED_LIBS)
    if (CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
       # Just setting CMAKE_POSITION_INDEPENDENT_CODE should be enough to set
       # -fPIC for GCC but sometimes it still doesn't get set, so make sure it
       # does.
       add_definitions("-fPIC")
    endif()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
else(BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-DQT_STATIC -DRABBITIM_STATIC)
endif(BUILD_SHARED_LIBS)

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckFunctionExists)
include(CheckCXXCompilerFlag)
include(CheckIncludeFiles)
include(CheckLibraryExists)

#CHECK_INCLUDE_FILE_CXX("string" HAVE_STRING_H)
#check_include_file("math.h" HAVE_MATH_H)

#check_function_exists("fabs" HAVE_FABS)

set(BUILD_PLATFORM "${CMAKE_SYSTEM_NAME}")
# ----------------------------------------------------------------------------
# Detect compiler and target platform architecture
# ----------------------------------------------------------------------------
if(NOT ANDROID)
    if(X86_64 OR CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(BUILD_ARCH x86_64)
    elseif(X86 OR CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(BUILD_ARCH x86)
    endif()
else()
    set(BUILD_ARCH ${CMAKE_SYSTEM_PROCESSOR})
endif()

if(NOT RabbitCommon_ROOT)
    set(RabbitCommon_ROOT $ENV{RabbitCommon_ROOT})
    if(NOT RabbitCommon_ROOT)
        set(RabbitCommon_ROOT ${CMAKE_SOURCE_DIR}/../RabbitCommon)
    endif()
endif()
if(DEFINED RabbitCommon_ROOT AND EXISTS ${RabbitCommon_ROOT}/Src)
    message("Use RabbitCommon source code")
    add_subdirectory(${RabbitCommon_ROOT}/Src ${CMAKE_BINARY_DIR}/RabbitCommon)
else()
    find_package(RabbitCommon)
    if(NOT RabbitCommon_FOUND)
        message(FATAL_ERROR
            "RabbitCommon_ROOT is not found. Please use one of the following ways to set it:\n"
            "1. Set RabbitCommon_ROOT to the install prefix of RabbitCommon.\n"
            "2. Set RabbitCommon_ROOT to source code root of RabbitCommon.\n"
            "2.1 Please download the source code of RabbitCommon from https://github.com/KangLin/RabbitCommon\n"
            "   ag:\n"
            "       git clone https://github.com/KangLin/RabbitCommon.git ${CMAKE_SOURCE_DIR}/..\n"
            "2.2 Then set cmake variable or environment variable RabbitCommon_ROOT to download root directory.\n"
            "   ag:\n"
            "       cmake -DRabbitCommon_ROOT=${CMAKE_SOURCE_DIR}/.. ")
    endif()
endif()

GET_VERSION(OUT_VERSION RabbitIm_VERSION
    OUT_REVISION RabbitIm_REVISION
    OUT_TAG RabbitImTag)
IF(NOT RabbitIm_VERSION)
    SET(RabbitIm_VERSION "0.1.0")
ENDIF()
if(NOT(RabbitIm_VERSION MATCHES "[0-9]+\\.[0-9]+\\.[0-9]+[+\\-^_~0-9A-Za-z]*"))
    SET(RabbitIm_VERSION "0.0.36")
endif()
IF(NOT RabbitImTag)
    SET(RabbitImTag "0.1.0")
ENDIF()
message("RabbitIm_VERSION:${RabbitIm_VERSION};Revision:${RabbitIm_REVISION};Tag:${RabbitImTag}")
set(VERSION ${RabbitIm_VERSION})

option(BUILD_PLUGIN_APP "Build plugin applications" OFF)
add_subdirectory(Src)
add_subdirectory(App)
add_subdirectory(Plugin)

option(BUILD_DOCS "Generating API documentation" OFF)
if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Doxygen)
        foreach(OUTPUT_LANGUAGE English Chinese)
            set(MAIN_README "README.md")
            if(OUTPUT_LANGUAGE STREQUAL "Chinese")
                set(MAIN_README "README_zh_CN.md")
            endif()            
            configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile_${OUTPUT_LANGUAGE} @ONLY)
            add_custom_target(doc_${OUTPUT_LANGUAGE} ALL
                ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile_${OUTPUT_LANGUAGE} # &> doxygen.log
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen" VERBATIM
                )
        endforeach()
        install(DIRECTORY ${CMAKE_BINARY_DIR}/Doxygen/
            DESTINATION ${CMAKE_INSTALL_DOCDIR}/${OUTPUT_LANGUAGE}
                COMPONENT Runtime)
    endif(DOXYGEN_FOUND)
endif(BUILD_DOCS)
