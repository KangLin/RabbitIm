cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

if(POLICY CMP0010)
  cmake_policy(SET CMP0010 NEW)
endif()

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

#判断 qt 版本
SET(QT_INSTALL_DIR "${Qt5_DIR}/../../..")
SET(QT_COMPONENTS Core Gui Widgets Network Xml Multimedia)
FIND_PACKAGE(Qt5 CONFIG REQUIRED ${QT_COMPONENTS} LinguistTools)
FOREACH(_COMPONENT ${QT_COMPONENTS})
    FIND_PACKAGE(Qt5${_COMPONENT} REQUIRED)
    SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5${_COMPONENT}_LIBRARIES})
ENDFOREACH()
FIND_PACKAGE(Qt5WebKitWidgets)
IF(Qt5WebKitWidgets_FOUND)
    ADD_DEFINITIONS(-DRABBITIM_WEBKIT)
    SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5WebKitWidgets_LIBRARIES})
ENDIF(Qt5WebKitWidgets_FOUND)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

include(CheckCXXCompilerFlag)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(cmake/RabbitImUtils.cmake)

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -static") 
ELSE(OPTION_RABBITIM_USE_STATIC)
    ADD_DEFINITIONS(-DQT_SHARED)
ENDIF(OPTION_RABBITIM_USE_STATIC)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    ADD_DEFINITIONS(-DDEBUG)# -DDEBUG_VIDEO_TIME )
ENDIF()

#设置头文件包含位置 
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
IF(MSVC)
    SET(RABBITIM_SYSTEM windows)
    SET(TOOLCHAIN_NAME windows_msvc)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")  #删除不是GBK字符的警告
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmtd /NODEFAULTLIB:libcmt /SUBSYSTEM:WINDOWS\",5.01\"")
ELSEIF(MINGW)
    SET(RABBITIM_SYSTEM windows)
    SET(TOOLCHAIN_NAME windows_mingw)
    # Windows compatibility
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
    # Statically link with libgcc
    # SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ ")
ELSEIF(ANDROID)
    SET(RABBITIM_SYSTEM android)
    SET(TOOLCHAIN_NAME android)
    ADD_DEFINITIONS(-DMOBILE)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/android/jni)
    FIND_PACKAGE(Qt5AndroidExtras REQUIRED)
    SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5AndroidExtras_LIBRARIES})
    #SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wno-psabi -march=armv7-a -mfloat-abi=softfp -mfpu=vfp -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums  -Wa,--noexecstack -gdwarf-2 -marm -fno-omit-frame-pointer -Wall -Wno-psabi -W -fPIE)
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined -Wl,-z,noexecstack -shared --sysroot=${ANDROID_SYSROOT}")
ELSE(LINUX OR UNIX)
    SET(RABBITIM_SYSTEM unix)
    ADD_DEFINITIONS(-DUNIX)
    SET(TOOLCHAIN_NAME unix)
ENDIF()
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} 
    ${CMAKE_SOURCE_DIR}/Widgets/FrmCustom
    ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/include)

SET(THIRD_PACKAGE_DIR ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}) #第三方开发包目录
IF(OPTION_RABBITIM_USE_STATIC AND EXISTS "${THIRD_PACKAGE_DIR}_static")
    SET(THIRD_PACKAGE_DIR ${THIRD_PACKAGE_DIR}_static)
ENDIF()
SET(THIRD_LIB_DIR ${THIRD_PACKAGE_DIR}/lib) #第三方库目录
LINK_DIRECTORIES(${THIRD_LIB_DIR})

#设置库的搜索路径
SET(CMAKE_PREFIX_PATH ${THIRD_LIB_DIR}/pkgconfig ${THIRD_PACKAGE_DIR} ${THIRD_LIB_DIR} ${CMAKE_PREFIX_PATH})
#允许 pkg-config 在 CMAKE_PREFIX_PATH 中搜索库
SET(PKG_CONFIG_USE_CMAKE_PREFIX_PATH TRUE)
#设置 find_XXX.cmake 搜索目录
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${THIRD_PACKAGE_DIR} ${CMAKE_MODULE_PATH})
IF(ANDROID)
    SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${THIRD_PACKAGE_DIR}/sdk/native/jni)
ENDIF(ANDROID)

message("CMAKE_PREFIX_PATH:${CMAKE_PREFIX_PATH}")
#pkgconfig模块
FIND_PACKAGE(PkgConfig)

# 依赖的第三方库
OPTION(OPTION_RABBITIM_USE_VPX "Use vpx library" ON)
IF(OPTION_RABBITIM_USE_VPX)
    SET(USE_VPX ON)

    IF(NOT MSVC AND PKG_CONFIG_FOUND)
        pkg_check_modules(VPX REQUIRED vpx)
        IF(VPX_FOUND)
            add_compile_options(${VPX_CFLAGS})
        ENDIF(VPX_FOUND)
    ELSE()
        MESSAGE("Use vpx library:${OPTION_RABBITIM_USE_VPX}\n")
        FIND_LIBRARY(VPX_LIBRARIES vpx)
    ENDIF()

    IF(ANDROID)
        FIND_LIBRARY(CPU_FEATURES cpu-features)
        SET(VPX_LIBRARIES ${VPX_LIBRARIES} ${CPU_FEATURES})
    ENDIF(ANDROID)
    SET(RABBITIM_PACKAGE_REQUIRES ${RABBITIM_PACKAGE_REQUIRES} libvpx-dev)
ELSE(OPTION_RABBITIM_USE_VPX)
    message("Must use libvpx, please install libvpx")
ENDIF(OPTION_RABBITIM_USE_VPX)

OPTION(OPTION_RABBITIM_USE_SPEEX "Use speex library" OFF)
MESSAGE("Use speex library:${OPTION_RABBITIM_USE_SPEEX}\n")
IF(OPTION_RABBITIM_USE_SPEEX)

    IF(PKG_CONFIG_FOUND)
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

#增加检测ffmpeg库
OPTION(OPTION_RABBITIM_USE_FFMPEG "Use ffmpeg library" ON)
IF(OPTION_RABBITIM_USE_FFMPEG)
    SET(RABBITIM_USE_FFMPEG ON)
    SET(RABBITIM_PACKAGE_REQUIRES "${RABBITIM_PACKAGE_REQUIRES}, libavcodec-dev, libavformat-dev, libavutil-dev, libswscale-dev")

    ADD_DEFINITIONS("-D__STDC_CONSTANT_MACROS" "-DRABBITIM_USE_FFMPEG") #ffmpeg需要
    IF(PKG_CONFIG_FOUND)
        find_package(FFMPEG REQUIRED COMPONENTS avcodec avformat avutil swscale)
        IF(FFMPEG_FOUND)
            INCLUDE_DIRECTORIES("${FFMPEG_INCLUDE_DIRS}")
        ENDIF(FFMPEG_FOUND)
    ELSEIF(PKG_CONFIG_FOUND)
        message("Use pkg-config fail, Manually set ffmpeg library.")
        FIND_LIBRARY(FFMPEG_LIBRARIES NAMES avcodec avformat avutil swscale swresample x264 pthread)
    ENDIF(PKG_CONFIG_FOUND)

ENDIF(OPTION_RABBITIM_USE_FFMPEG)
MESSAGE("Use ffmpeg library:${RABBITIM_USE_FFMPEG}\n")

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
MESSAGE("Use opencv library:${RABBITIM_USE_OPENCV}\n")

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
            message("OPENSSL_CFLAGS:${OPENSSL_CFLAGS}")
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
MESSAGE("Use openssl library:${RABBITIM_USE_OPENSSL}\n")

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
            message("CURL_CFLAGS:${CURL_CFLAGS}")
            add_compile_options(${CURL_CFLAGS})
        ELSE(CURL_FOUND)
            SET(RABBITIM_USE_LIBCURL OFF)
        ENDIF(CURL_FOUND)
    ELSE()
        find_package(CURL)
        if(CURL_FOUND)
            message("CURL_INCLUDE_DIR:${CURL_INCLUDE_DIR}")
            include_directories(${CURL_INCLUDE_DIR})
        ELSE(CURL_FOUND)
            SET(RABBITIM_USE_LIBCURL OFF)
        endif(CURL_FOUND)
    ENDIF()

ENDIF(OPTION_RABBITIM_USE_LIBCURL)
MESSAGE("Use curl library:${RABBITIM_USE_LIBCURL}\n")

#qxmpp库
#连接静态QXMPP库时，必须加上-DQXMPP_STATIC。生成静态QXMPP库时，qmake 需要加上 QXMPP_LIBRARY_TYPE=staticlib 参数
ADD_DEFINITIONS("-DQXMPP") # -DQXMPP_STATIC")
IF(OPTION_RABBITIM_USE_STATIC)
    ADD_DEFINITIONS("-DQXMPP_STATIC")
ENDIF()
IF(ANDROID)
    ADD_DEFINITIONS("-DQXMPP_STATIC")
ENDIF(ANDROID)
IF(PKG_CONFIG_FOUND AND NOT MSVC)
    pkg_check_modules(QXMPP REQUIRED qxmpp)
    IF(QXMPP_FOUND)
        add_compile_options(${QXMPP_CFLAGS})
    ENDIF(QXMPP_FOUND)
ELSE()
    IF(OPTION_RABBITIM_USE_STATIC)
        FIND_LIBRARY(QXMPP_LIBRARIES qxmpp)
    ELSE()
        FIND_LIBRARY(QXMPP_LIBRARIES qxmpp0)
    ENDIF()
ENDIF()

SET(RABBITIM_LIBS
    ${RABBITIM_LIBS}
    ${QXMPP_LIBRARIES}
    ${OpenCV_LIBS}
    ${FFMPEG_LIBRARIES}
    ${VPX_LIBRARIES}
    ${SPEEX_LIBRARIES}
    ${CURL_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${QT_LIBRARIES}
    )

include(cmake/install_version.cmake)
include(cmake/cmake_build_file.cmake)

SET(RABBITIM_RCC_FILES Resource/Resource.qrc Resource/sink/dark/style.qrc AppIcon.rc)

#生成目标
IF(ANDROID)
    add_library(${PROJECT_NAME} SHARED 
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        ${RABBITIM_UIS}
        )
    #qt5_wrap_ui(RABBITIM_UIS ${RABBITIM_UIS})
ELSEIF(WIN32)
    add_executable(${PROJECT_NAME} WIN32
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        ${RABBITIM_UIS}
        )
ELSE()
    add_executable(${PROJECT_NAME} 
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        ${RABBITIM_UIS}
        )
ENDIF()
message("RABBITIM_LIBS:${RABBITIM_LIBS}\n")
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
        FILE(COPY ${QT_INSTALL_DIR}/translations/qt_zh_CN.qm DESTINATION "translations")

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
        SET(QT_PLUGINS Qt5Core_PLUGINS 
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
