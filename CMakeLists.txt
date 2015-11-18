cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(RabbitIm)

# 建立要删除的文件
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
# 建立删除目标
ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")

# 设置安装前缀
IF(NOT CMAKE_INSTALL_PREFIX) 
    SET(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/${PROJECT_NAME} CACHE STRING "Install path prefix.") 
ENDIF() 

# Find Git Version Patch
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    FIND_PROGRAM(GIT git)
    IF(GIT)
        EXECUTE_PROCESS(
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND ${GIT} rev-parse --short HEAD
            OUTPUT_VARIABLE BUILD_VERSION  OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        MESSAGE("Git Version: ${BUILD_VERSION}\n")
    ENDIF(GIT)
ENDIF()
MESSAGE("\n")

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

#判断 qt 版本
SET(QT_INSTALL_DIR "${Qt5_DIR}/../../..")
#需要的QT组件  
SET(QT_COMPONENTS Core Gui Widgets Network Xml Multimedia)
FOREACH(_COMPONENT ${QT_COMPONENTS})
    FIND_PACKAGE(Qt5${_COMPONENT} REQUIRED)
    SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5${_COMPONENT}_LIBRARIES})
ENDFOREACH()
FIND_PACKAGE(Qt5WebKitWidgets)
IF(Qt5WebKitWidgets_FOUND)
    ADD_DEFINITIONS(-DRABBITIM_WEBKIT)
    SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5WebKitWidgets_LIBRARIES})
ENDIF(Qt5WebKitWidgets_FOUND)
FIND_PACKAGE(Qt5 CONFIG REQUIRED LinguistTools) #语言工具  

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

include(CheckCXXCompilerFlag)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(cmake/RabbitImUtils.cmake)

# Exit for blacklisted compilers (those that don't support C++11 very well)
#  MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
#  Clang 3.0
SET(BAD_CXX_MESSAGE "")
IF(MSVC)
    IF(MSVC_VERSION LESS 1500)
      SET(BAD_CXX_MESSAGE "MSVC 2008 or higher")
    ENDIF()
ENDIF()
IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    IF(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 3.1.0)
      SET(BAD_CXX_MESSAGE "Clang v3.1.0 or higher")
    ENDIF()
ENDIF()
IF(BAD_CXX_MESSAGE)
    MESSAGE(FATAL_ERROR "\nSorry, ${BAD_CXX_MESSAGE} is required to build this software. Please retry using a modern compiler that supports C++11 lambdas.")
ENDIF()
IF(NOT MSVC)
    SET(CMAKE_CXX_FLAGS "-std=c++0x") #启用C++11
ENDIF(NOT MSVC)

#
# Compiler & linker settings
#
if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  check_cxx_compiler_flag(-Wunused-but-set-variable HAVE_GXX_UNUSED_BUT_SET)
  check_cxx_compiler_flag(-Wlogical-op HAVE_GXX_LOGICAL_OP)
  check_cxx_compiler_flag(-Wsizeof-pointer-memaccess HAVE_GXX_POINTER_MEMACCESS)
  check_cxx_compiler_flag(-Wreorder HAVE_GXX_REORDER)
  check_cxx_compiler_flag(-Wformat-security HAVE_GXX_FORMAT_SECURITY)
  check_cxx_compiler_flag(-std=c++0x HAVE_GXX_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wextra -Woverloaded-virtual -Winit-self -Wmissing-include-dirs -Wunused -Wno-div-by-zero -Wundef -Wpointer-arith -Wmissing-noreturn -Werror=return-type")
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++") # required in C++11 mode
  endif()
  if(HAVE_GXX_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  endif()
  if(HAVE_GXX_UNUSED_BUT_SET)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-but-set-variable")
  endif()
  if(HAVE_GXX_LOGICAL_OP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wlogical-op")
  endif()
  if(HAVE_GXX_POINTER_MEMACCESS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsizeof-pointer-memaccess")
  endif()
  if(HAVE_GXX_REORDER)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wreorder")
  endif()
  if(HAVE_GXX_FORMAT_SECURITY)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat -Wformat-security")
  endif()
  if(MINGW)
    # mingw will error out on the crazy casts in probe.cpp without this
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
  else()
    # visibility attributes not supported on mingw, don't use -fvisibility option
    # see: http://stackoverflow.com/questions/7994415/mingw-fvisibility-hidden-does-not-seem-to-work
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
  endif()
endif()

if(WIN32)
  add_definitions(-DUNICODE -D_UNICODE)
endif()

# linker flags
if(CMAKE_SYSTEM_NAME MATCHES Linux OR CMAKE_SYSTEM_NAME STREQUAL GNU)
  if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--fatal-warnings -Wl,--no-undefined -lc ${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--fatal-warnings -Wl,--no-undefined -lc ${CMAKE_MODULE_LINKER_FLAGS}")
  endif()
endif()

# Add definitions for static/style library 
OPTION(OPTION_RABBITIM_USE_STATIC "Build statically" OFF)
MESSAGE("Build statically: ${OPTION_RABBITIM_USE_STATIC}")
IF(OPTION_RABBITIM_USE_STATIC)
    ADD_DEFINITIONS(-DQT_STATIC)
    set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -static") 
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
ELSE(OPTION_RABBITIM_USE_STATIC)
    ADD_DEFINITIONS(-DQT_SHARED)
ENDIF(OPTION_RABBITIM_USE_STATIC)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    ADD_DEFINITIONS(-DDEBUG) # -DDEBUG_VIDEO_TIME )
ENDIF()

IF(MSVC)
    SET(RABBITIM_SYSTEM windows)
    SET(TOOLCHAIN_NAME windows_msvc)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")  #删除不是GBK字符的警告
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmtd /NODEFAULTLIB:libcmt /SUBSYSTEM:WINDOWS\",5.01\"")
    ADD_DEFINITIONS(-DWINDOWS)
ELSEIF(MINGW)
    SET(RABBITIM_SYSTEM windows)
    SET(TOOLCHAIN_NAME windows_mingw)
    # Windows compatibility
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
    # Statically link with libgcc
    #SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ ")
    ADD_DEFINITIONS(-DWINDOWS)
ELSEIF(ANDROID)
    SET(RABBITIM_SYSTEM android)
    SET(TOOLCHAIN_NAME android)
    SET(OPTION_RABBITIM_USE_STATIC ON) #android用静态编译  
    ADD_DEFINITIONS(-DMOBILE)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/android/jni)
    FIND_PACKAGE(Qt5AndroidExtras REQUIRED)
    SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5AndroidExtras_LIBRARIES})
    #SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wno-psabi -march=armv7-a -mfloat-abi=softfp -mfpu=vfp -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums  -Wa,--noexecstack -gdwarf-2 -marm -fno-omit-frame-pointer -Wall -Wno-psabi -W -fPIE)
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined -Wl,-z,noexecstack -shared --sysroot=${ANDROID_SYSROOT}")
ELSE(LINUX OR UNIX)
    SET(RABBITIM_SYSTEM unix)
    SET(TOOLCHAIN_NAME unix)
    ADD_DEFINITIONS(-DUNIX)
ENDIF()
IF(NOT DEFINED RABBITIM_THIRD_PACKAGE_DIR)
    SET(RABBITIM_THIRD_PACKAGE_DIR ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}) #第三方开发包目录
ENDIF()
IF(OPTION_RABBITIM_USE_STATIC AND EXISTS "${RABBITIM_THIRD_PACKAGE_DIR}_static")
    SET(RABBITIM_THIRD_PACKAGE_DIR ${RABBITIM_THIRD_PACKAGE_DIR}_static)
ENDIF()
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} 
    ${CMAKE_SOURCE_DIR}/Widgets/FrmCustom
    ${RABBITIM_THIRD_PACKAGE_DIR}/include)        #第三方包含头文件目录
SET(THIRD_LIB_DIR ${RABBITIM_THIRD_PACKAGE_DIR}/lib) #第三方库目录
LINK_DIRECTORIES(${THIRD_LIB_DIR})

#pkgconfig模块
#IF(MINGW)
#    set(ENV{PKG_CONFIG_PATH} "${THIRD_LIB_DIR}/pkgconfig" ENV{PKG_CONFIG_PATH})
#ELSEIF(ANDROID)
#    set(ENV{PKG_CONFIG_PATH} "${THIRD_LIB_DIR}/pkgconfig")
#    set(ENV{PKG_CONFIG_SYSROOT_DIR} "${RABBITIM_THIRD_PACKAGE_DIR}")
#    set(ENV{PKG_CONFIG_LIBDIR} "${THIRD_LIB_DIR}/pkgconfig")
#ELSE()
#    set(ENV{PKG_CONFIG_PATH} "${THIRD_LIB_DIR}/pkgconfig" ENV{PKG_CONFIG_PATH})
#ENDIF()

#设置 find_package 搜索目录,(find_XXX.cmake)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${RABBITIM_THIRD_PACKAGE_DIR} ${CMAKE_MODULE_PATH})
FIND_PACKAGE(PkgConfig)
#允许 pkg-config 在 CMAKE_PREFIX_PATH 中搜索库
SET(PKG_CONFIG_USE_CMAKE_PREFIX_PATH TRUE)
#设置库的搜索路径
SET(CMAKE_PREFIX_PATH ${THIRD_LIB_DIR}/pkgconfig ${THIRD_LIB_DIR} ${RABBITIM_THIRD_PACKAGE_DIR} ${CMAKE_PREFIX_PATH})
IF(ANDROID)
    SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${RABBITIM_THIRD_PACKAGE_DIR}/sdk/native/jni)
    SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${RABBITIM_THIRD_PACKAGE_DIR}/libs/${ANDROID_ABI}/pkgconfig)
    SET(THIRD_LIB_DIR ${THIRD_LIB_DIR} ${RABBITIM_THIRD_PACKAGE_DIR}/libs/${ANDROID_ABI})
ENDIF(ANDROID)
message("CMAKE_PREFIX_PATH:${CMAKE_PREFIX_PATH}")

# 依赖的第三方库
OPTION(OPTION_RABBITIM_USE_VPX "Use vpx library" ON)
IF(OPTION_RABBITIM_USE_VPX)
    SET(USE_VPX ON)
    IF(PKG_CONFIG_FOUND AND NOT MSVC)
        pkg_check_modules(VPX REQUIRED vpx)
        IF(VPX_FOUND)
            add_compile_options(${VPX_CFLAGS})
        ENDIF(VPX_FOUND)
    ELSE()
        MESSAGE("Use vpx library:${OPTION_RABBITIM_USE_VPX}")
        FIND_LIBRARY(VPX_LIBRARIES vpx)
    ENDIF()

    IF(ANDROID)
        FIND_LIBRARY(CPU_FEATURES cpu-features)
        if(${CPU_FEATURES} STREQUAL "CPU_FEATURES-NOTFOUND")
            SET(CPU_FEATURES cpu-features)
        endif()
        SET(VPX_LIBRARIES ${VPX_LIBRARIES} ${CPU_FEATURES})
    ENDIF(ANDROID)
    SET(RABBITIM_PACKAGE_REQUIRES ${RABBITIM_PACKAGE_REQUIRES} libvpx-dev)
ELSE(OPTION_RABBITIM_USE_VPX)
    message("Must use libvpx, please install libvpx")
ENDIF(OPTION_RABBITIM_USE_VPX)

OPTION(OPTION_RABBITIM_USE_SPEEX "Use speex library" OFF)
MESSAGE("Use speex library:${OPTION_RABBITIM_USE_SPEEX}")
IF(OPTION_RABBITIM_USE_SPEEX)

    IF(PKG_CONFIG_FOUND AND NOT MSVC)
        pkg_check_modules(SPEEX REQUIRED speex)
        IF(SPEEX_FOUND)
            add_compile_options(${SPEEX_CFLAGS})
        ELSEIF(SPEEX_FOUND)
            FIND_LIBRARY(SPEEX_LIBRARIES NAMES speex speexdsp)
        ENDIF(SPEEX_FOUND)
    ELSE(PKG_CONFIG_FOUND)
        FIND_LIBRARY(SPEEX_LIBRARIES NAMES speex speexdsp)
    ENDIF(PKG_CONFIG_FOUND)

    SET(RABBITIM_PACKAGE_REQUIRES "${RABBITIM_PACKAGE_REQUIRES},libspeex1,libspeexdsp1")
ENDIF(OPTION_RABBITIM_USE_SPEEX)

#libqrencode 库  
IF(PKG_CONFIG_FOUND AND NOT MSVC)
    pkg_check_modules(LIBQRENCODE libqrencode)
    IF(LIBQRENCODE_FOUND)
        IF(OPTION_RABBITIM_USE_STATIC)
            add_compile_options(${LIBQRENCODE_STATIC_CFLAGS})
            SET(LIBQRENCODE_LIBRARIES ${LIBQRENCODE_STATIC_LIBRARIES})
        ELSE()
            add_compile_options(${LIBQRENCODE_CFLAGS})
        ENDIF()
        ADD_DEFINITIONS(-DRABBITIM_USE_LIBQRENCODE)
    ENDIF()
ENDIF()

#QZXing库  
IF(PKG_CONFIG_FOUND AND NOT MSVC)
    pkg_check_modules(QZXING QZXing)
    IF(QZXING_FOUND)
        ADD_DEFINITIONS(-DRABBITIM_USE_QZXING)
        IF(OPTION_RABBITIM_USE_STATIC)
            add_compile_options(${QZXING_STATIC_CFLAGS})
            SET(QZXING_LIBRARIES ${QZXING_STATIC_LIBRARIES})
        ELSE()
            add_compile_options(${QZXING_CFLAGS})
        ENDIF()
    ENDIF()
ELSE()
    FIND_LIBRARY(QZXING_LIBRARIES QZXing2)
ENDIF()

#增加检测ffmpeg库
OPTION(OPTION_RABBITIM_USE_FFMPEG "Use ffmpeg library" ON)
IF(OPTION_RABBITIM_USE_FFMPEG)
    SET(RABBITIM_USE_FFMPEG ON)
    SET(RABBITIM_PACKAGE_REQUIRES "${RABBITIM_PACKAGE_REQUIRES}, libavcodec-dev, libavformat-dev, libavutil-dev, libswscale-dev")

    ADD_DEFINITIONS("-D__STDC_CONSTANT_MACROS" "-DRABBITIM_USE_FFMPEG") #ffmpeg需要
    IF(PKG_CONFIG_FOUND AND NOT MSVC)
        pkg_check_modules(FFMPEG REQUIRED libavcodec libavformat libavutil libswscale)
        IF(FFMPEG_FOUND)
            #INCLUDE_DIRECTORIES("${FFMPEG_INCLUDE_DIRS}")
            IF(OPTION_RABBITIM_USE_STATIC)
                #INCLUDE_DIRECTORIES("${FFMPEG_STATIC_INCLUDE_DIRS}")
                SET(FFMPEG_LIBRARIES ${FFMPEG_STATIC_LIBRARIES})
            ENDIF()
        ELSE()
            find_package(FFMPEG REQUIRED avcodec avformat avutil swscale)
        ENDIF(FFMPEG_FOUND)
    ELSEIF()
        message("Use pkg-config fail, Manually set ffmpeg library.")
        FIND_LIBRARY(FFMPEG_LIBRARIES NAMES avcodec avformat avutil swscale swresample x264 pthread)
    ENDIF()

ENDIF(OPTION_RABBITIM_USE_FFMPEG)
MESSAGE("Use ffmpeg library:${RABBITIM_USE_FFMPEG}")

#增加检测opencv库
OPTION(OPTION_RABBITIM_USE_OPENCV "Use opencv library" ON)
SET(RABBITIM_USE_OPENCV OFF)
IF(OPTION_RABBITIM_USE_OPENCV)
    find_package(OpenCV COMPONENTS core imgproc video) # videoio)
    IF(OpenCV_FOUND)
        SET(RABBITIM_USE_OPENCV ON)
        INCLUDE_DIRECTORIES("${OpenCV_INCLUDE_DIRS}")
        LINK_DIRECTORIES(${OpenCV_LIB_DIR})
    ENDIF()
ELSEIF(OPTION_RABBITIM_USE_OPENCV)
    IF(ANDROID)
        message("android must opencv library, please install library, and -DOPTION_RABBITIM_USE_OPENCV=ON")
    ENDIF(ANDROID)
ENDIF(OPTION_RABBITIM_USE_OPENCV)
IF(RABBITIM_USE_OPENCV)
    add_definitions("-DRABBITIM_USE_OPENCV")
ENDIF(RABBITIM_USE_OPENCV)
MESSAGE("Use opencv library:${RABBITIM_USE_OPENCV}")

#检测openssl库
OPTION(OPTION_RABBITIM_USE_OPENSSL "Use openssl library" ON)
SET(RABBITIM_USE_OPENSSL OFF)
IF(OPTION_RABBITIM_USE_OPENSSL)
    SET(RABBITIM_USE_OPENSSL ON)
    ADD_DEFINITIONS("-DRABBITIM_USE_OPENSSL")
    SET(RABBITIM_PACKAGE_REQUIRES "${RABBITIM_PACKAGE_REQUIRES}, libssl-dev")

    IF(PKG_CONFIG_FOUND AND NOT MSVC)
        pkg_check_modules(OPENSSL openssl)
        IF(OPENSSL_FOUND)
            IF(OPTION_RABBITIM_USE_STATIC)
                add_compile_options(${OPENSSL_STATIC_CFLAGS})
                SET(OPENSSL_LIBRARIES ${OPENSSL_STATIC_LIBRARIES})
            ELSE()
                add_compile_options(${OPENSSL_CFLAGS})
            ENDIF()
            add_compile_options(${OPENSSL_CFLAGS})
        ELSE(OPENSSL_FOUND)
            SET(RABBITIM_USE_OPENSSL OFF)
        ENDIF(OPENSSL_FOUND)
    ELSE()
        find_package(OpenSSL)
        IF(OPENSSL_FOUND)
            message("OPENSSL_INCLUDE_DIR:${OPENSSL_INCLUDE_DIR}")
            include_directories(${OPENSSL_INCLUDE_DIR})
        ELSE(OPENSSL_FOUND)
            SET(RABBITIM_USE_OPENSSL OFF)
        ENDIF(OPENSSL_FOUND)
    ENDIF()

ENDIF(OPTION_RABBITIM_USE_OPENSSL)
MESSAGE("Use openssl library:${RABBITIM_USE_OPENSSL}")

#检测libcurl库  
OPTION(OPTION_RABBITIM_USE_LIBCURL "Use curl library" ON)
SET(RABBITIM_USE_LIBCURL OFF)
IF(OPTION_RABBITIM_USE_LIBCURL)
    SET(RABBITIM_USE_LIBCURL ON)
    add_definitions("-DRABBITIM_USE_LIBCURL")
    SET(RABBITIM_PACKAGE_REQUIRES "${RABBITIM_PACKAGE_REQUIRES}, libcurl4-openssl-dev")

    IF(PKG_CONFIG_FOUND AND NOT MSVC)
        pkg_check_modules(CURL libcurl)
        IF(CURL_FOUND)
            IF(OPTION_RABBITIM_USE_STATIC)
                add_compile_options(${CURL_STATIC_CFLAGS})
                SET(CURL_LIBRARIES ${CURL_LIBRARIES} ${CURL_STATIC_LIBRARIES})
            ELSE()
                add_compile_options(${CURL_CFLAGS})
            ENDIF()
        ELSE(CURL_FOUND)
            SET(RABBITIM_USE_LIBCURL OFF)
        ENDIF(CURL_FOUND)
    ELSE()
        find_package(CURL)
        if(CURL_FOUND)
            message("CURL_INCLUDE_DIR:${CURL_INCLUDE_DIR}")
            include_directories(${CURL_INCLUDE_DIR})
            message("CURL_CFLAGS:${CURL_CFLAGS}")
            add_compile_options(-DCURL_STATICLIB)
        ELSE(CURL_FOUND)
            SET(RABBITIM_USE_LIBCURL OFF)
        endif(CURL_FOUND)
    ENDIF()

ENDIF(OPTION_RABBITIM_USE_LIBCURL)
MESSAGE("Use curl library:${RABBITIM_USE_LIBCURL}")

#qxmpp库
OPTION(OPTION_RABBITIM_USE_QXMPP "Use qxmpp library" ON)
if(OPTION_RABBITIM_USE_QXMPP)
    #连接静态QXMPP库时，必须加上-DQXMPP_STATIC。
    #生成静态QXMPP库时，qmake 需要加上 QXMPP_LIBRARY_TYPE=staticlib 参数
    ADD_DEFINITIONS("-DRABBITIM_USE_QXMPP") 
    IF(OPTION_RABBITIM_USE_STATIC)
        ADD_DEFINITIONS("-DQXMPP_STATIC")
    ENDIF()

    SET(QXMPP_NAME qxmpp)
    IF(CMAKE_BUILD_TYPE MATCHES Debug)
        SET(QXMPP_NAME qxmpp_d)
    ENDIF()
    IF(PKG_CONFIG_FOUND AND NOT MSVC)
        pkg_check_modules(QXMPP REQUIRED ${QXMPP_NAME})
        IF(QXMPP_FOUND)
            IF(OPTION_RABBITIM_USE_STATIC)
                add_compile_options(${QXMPP_STATIC_CFLAGS})
                SET(QXMPP_LIBRARIES ${QXMPP_STATIC_LIBRARIES})
            ELSE()
                add_compile_options(${QXMPP_CFLAGS})
            ENDIF()
        ENDIF(QXMPP_FOUND)
    ELSE()
        IF(OPTION_RABBITIM_USE_STATIC)
            FIND_LIBRARY(QXMPP_LIBRARIES qxmpp)
        ELSE(OPTION_RABBITIM_USE_STATIC)
            FIND_LIBRARY(QXMPP_LIBRARIES qxmpp0)
        ENDIF(OPTION_RABBITIM_USE_STATIC)
    ENDIF()
ENDIF(OPTION_RABBITIM_USE_QXMPP)
message("Use qxmpp library:${OPTION_RABBITIM_USE_QXMPP}")

OPTION(OPTION_RABBITIM_USE_PJSIP "Use pjsip library" OFF)
SET(RABBITIM_USE_PJSIP OFF)
IF(OPTION_RABBITIM_USE_PJSIP)
    SET(RABBITIM_USE_PJSIP ON)
    IF(PKG_CONFIG_FOUND AND NOT MSVC)
        pkg_check_modules(PJSIP REQUIRED libpjproject)
        IF(PJSIP_FOUND)
            add_compile_options(${PJSIP_CFLAGS})
        ENDIF(PJSIP_FOUND)
    ELSE()
        SET(RABBITIM_USE_PJSIP OFF)
    ENDIF()
ENDIF()
message("Use PJSIP library:${RABBITIM_USE_PJSIP}")

include(cmake/install_version.cmake)
include(cmake/cmake_build_file.cmake)
include(Plugin/CMakeLists.txt)

SET(RABBITIM_LIBS
    ${RABBITIM_LIBS}
    ${QXMPP_LIBRARIES}
    ${LIBQRENCODE_LIBRARIES}
    ${QZXING_LIBRARIES}
    ${OpenCV_LIBS}
    ${FFMPEG_LIBRARIES}
    ${VPX_LIBRARIES}
    ${SPEEX_LIBRARIES}
    ${CURL_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${QT_LIBRARIES}
    )

#设置资源文件
SET(RABBITIM_RCC_FILES
    ${RABBITIM_RCC_FILES}
    Resource/Resource.qrc
    Resource/sink/dark/style.qrc
    AppIcon.rc)

#生成目标
IF(WIN32)
    add_executable(${PROJECT_NAME} WIN32
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        ${RABBITIM_UIS}
        )
ELSEIF(ANDROID)
    add_library(${PROJECT_NAME} SHARED 
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        ${RABBITIM_UIS}
        )
    #qt5_wrap_ui(RABBITIM_UIS ${RABBITIM_UIS})
ELSE()
    add_executable(${PROJECT_NAME} 
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        ${RABBITIM_UIS}
        )
ENDIF()
message("RABBITIM_LIBS:${RABBITIM_LIBS}")
#链接库
target_link_libraries(${PROJECT_NAME} 
        ${RABBITIM_LIBS}
        )

#翻译
SET(TS_FILES Resource/translations/app_zh_CN.ts)
OPTION(RABBITIM_TRANSLATIONS "Refresh translations on compile" ON)
MESSAGE("Refresh translations on compile: ${RABBITIM_TRANSLATIONS}\n")
IF(RABBITIM_TRANSLATIONS)
    IF(NOT Qt5_LRELEASE_EXECUTABLE)
        MESSAGE(WARNING "Could not find lrelease. Your build won't contain translations.")
    ELSE(NOT Qt5_LRELEASE_EXECUTABLE)
        qt5_add_translation(QM_FILES ${TS_FILES}) #生成翻译资源文件  
        ADD_CUSTOM_TARGET(translations ALL DEPENDS ${QM_FILES})
        add_dependencies(${PROJECT_NAME} translations)
        foreach(_file ${QM_FILES})
            IF(ANDROID)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        #COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/android-build/translations
                        COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${PROJECT_BINARY_DIR}/android-build/translations
                        COMMAND ${CMAKE_COMMAND} -E copy ${QT_INSTALL_DIR}/translations/qt_zh_CN.qm ${PROJECT_BINARY_DIR}/android-build/translations
                        )
            ELSE(ANDROID)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E make_directory translations
                        COMMAND ${CMAKE_COMMAND} -E copy ${_file} translations
                        )
            ENDIF(ANDROID)
        endforeach()
        IF(EXISTS "${QT_INSTALL_DIR}/translations/qt_zh_CN.qm")
            FILE(COPY ${QT_INSTALL_DIR}/translations/qt_zh_CN.qm DESTINATION "translations")
        ENDIF()

        #安装1:翻译
        INSTALL(FILES "${QT_INSTALL_DIR}/translations/qt_zh_CN.qm" DESTINATION "translations" CONFIGURATIONS Release)
        INSTALL(FILES "${QM_FILES}" DESTINATION "translations" CONFIGURATIONS Release)
    ENDIF(NOT Qt5_LRELEASE_EXECUTABLE)
ENDIF(RABBITIM_TRANSLATIONS)

#只发行版本才安装
IF(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    #安装2:目标
    IF(ANDROID)
        INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}")
    ELSE(ANDROID)
        INSTALL(TARGETS "${PROJECT_NAME}" RUNTIME DESTINATION "." CONFIGURATIONS Release)
    ENDIF(ANDROID)

    #安装3:qt库
    IF(WIN32 AND CMAKE_HOST_WIN32)
        IF(MINGW)
            #windeployqt 分发时，是根据是否 strip 来判断是否是 DEBUG 版本,而用mingw编译时,qt没有自动 strip
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND strip "${PROJECT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
                )
        ENDIF()

        #add_custom_target(Deployment DEPENDS ${PROJECT_NAME}
        #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/${PROJECT_NAME}"
            COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/."
            COMMAND "${QT_INSTALL_DIR}/bin/windeployqt"
                --compiler-runtime
                --verbose 7
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
        )
        INSTALL(DIRECTORY "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/." DESTINATION ".")
        #install(CODE "execute_process(COMMAND \"${QT_INSTALL_DIR}/bin/windeployqt\"
        #                    --compiler-runtime
        #                    --no-translations
        #                    --verbose 7
        #                    \"${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}\"
        #                    WORKING_DIRECTORY .)"
        #        )
    ELSEIF(NOT ANDROID)
        SET(QT_PLUGINS ${QT_PLUGINS} Qt5Core_PLUGINS 
                       Qt5Gui_PLUGINS
                       Qt5Widgets_PLUGINS
                       Qt5Network_PLUGINS
                       Qt5Multimedia_PLUGINS
                       Qt5Xml_PLUGINS)
        IF(Qt5WebKitWidgets_FOUND)
            SET(QT_PLUGINS ${QT_PLUGINS} Qt5WebKitWidgets_PLUGINS)
        ENDIF(Qt5WebKitWidgets_FOUND)
        INSTALL_QT_LIBRARYS(${QT_LIBRARIES})
        INSTALL_QT_PLUGINS(${QT_PLUGINS})
    ENDIF()
ENDIF(NOT CMAKE_BUILD_TYPE MATCHES Debug)

IF(ANDROID)
    GENERATED_DEPLOYMENT_SETTINGS()
    find_program(ANT NAMES ant) # PATHS  "/usr/bin")
    MESSAGE("ant:${ANT}\n")

    add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/libRabbitIm.so ${PROJECT_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/libRabbitIm.so
        #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
        COMMAND "${QT_INSTALL_DIR}/bin/androiddeployqt"    
            --input ${PROJECT_BINARY_DIR}/android-libRabbitIm.so-deployment-settings.json
            --output ${PROJECT_BINARY_DIR}/android-build
            --verbose
            #--ant /usr/bin/ant
            #--jdk $ENV{JAVA_HOME}
    )
ENDIF(ANDROID)

SET(CMAKE_INSTALL_RPATH ".") #设置链接库的搜索路径
#复制第三方依赖库的动态库到编译目录下
IF(WIN32)
    FILE(GLOB THIRD_DLL_LIBRARY_BIN "${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/bin/*.dll")
    FILE(GLOB THIRD_DLL_LIBRARY_LIB "${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/lib/*.dll")
    SET(THIRD_DLL_LIBRARY "${THIRD_DLL_LIBRARY_BIN} ${THIRD_DLL_LIBRARY_LIB}")
    #安装4:第三方动态库
    INSTALL(DIRECTORY "${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/bin/" 
            DESTINATION .
            FILES_MATCHING PATTERN "*.dll")
ELSEIF(NOT ANDROID)
    FILE(GLOB THIRD_DLL_LIBRARY "${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/lib/*.so")
    #安装4:第三方动态库
    INSTALL(DIRECTORY "${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/lib/"
            DESTINATION "."
            FILES_MATCHING PATTERN "*.so*")
ENDIF(WIN32)
#MESSAGE("THIRD_DLL_LIBRARY:${THIRD_DLL_LIBRARY}")
FOREACH(file ${THIRD_DLL_LIBRARY})
    IF(EXISTS "${file}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${file}" "${PROJECT_BINARY_DIR}/."
            )
     ENDIF(EXISTS "${file}")
ENDFOREACH(file)

#安装5:安装相关文件
install(FILES License.md Authors.txt ChangeLog.md DESTINATION .)
