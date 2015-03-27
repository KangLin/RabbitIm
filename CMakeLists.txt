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
FIND_PACKAGE(Qt5 5.3 CONFIG REQUIRED Gui Core Network Xml Multimedia Widgets LinguistTools)
SET(QT_INSTALL_DIR "${Qt5_DIR}/../../..")
FIND_PACKAGE(Qt5WebKitWidgets)
IF(Qt5WebKitWidgets_FOUND)
	ADD_DEFINITIONS(-DRABBITIM_WEBKIT)
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
OPTION(OPTION_STATIC "Build statically" OFF)
MESSAGE("Build statically: ${OPTION_STATIC}")
IF(OPTION_STATIC)
    ADD_DEFINITIONS(-DQT_STATIC)
ELSE(OPTION_STATIC)
    ADD_DEFINITIONS(-DQT_SHARED)
ENDIF(OPTION_STATIC)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    ADD_DEFINITIONS(-DDEBUG)# -DDEBUG_VIDEO_TIME )
ENDIF()

#设置头文件包含位置 
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
IF(MSVC)
    SET(RABBITIM_SYSTEM windows)
    SET(TOOLCHAIN_NAME windows_msvc)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")  #删除不是GBK字符的警告
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmtd /NODEFAULTLIB:libcmt")
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
    FIND_PACKAGE(Qt5 CONFIG REQUIRED  AndroidExtras)
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

SET(THIRD_PACKAGE ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME})   #第三方开发包目录  
SET(THIRD_LIB_DIR ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/lib) #第三方库目录
LINK_DIRECTORIES(${THIRD_LIB_DIR})
#设置库的搜索路径  
SET(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME} ${CMAKE_PREFIX_PATH})
#MESSAGE("CMAKE_PREFIX_PATH:${CMAKE_PREFIX_PATH}/lib/pkgconfig")
#允许 pkg-config 在 CMAKE_PREFIX_PATH 中搜索库
SET(PKG_CONFIG_USE_CMAKE_PREFIX_PATH TRUE)
#设置 find_XXX.cmake 搜索目录  
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${THIRD_LIB_DIR}/cmake ${CMAKE_MODULE_PATH})
#pkgconfig模块
FIND_PACKAGE(PkgConfig)

# 依赖的第三方库
OPTION(OPTION_RABBITIM_USE_VPX "Use vpx library" ON)
MESSAGE("Use vpx library:${OPTION_RABBITIM_USE_VPX}\n")
IF(OPTION_RABBITIM_USE_VPX)
    SET(USE_VPX ON)

    IF(PKG_CONFIG_FOUND)
        pkg_check_modules(VPX REQUIRED vpx)
        IF(VPX_FOUND)
            add_compile_options(${VPX_CFLAGS})
        ELSEIF(VPX_FOUND)
            SET(VPX_LIBRARIES -lvpx)
        ENDIF(VPX_FOUND)
    ELSE(PKG_CONFIG_FOUND)
        SET(VPX_LIBRARIES -lvpx)
        IF(ANDROID)
            SET(VPX_LIBRARIES ${VPX_LIBRARIES} cpu-features)
        ENDIF(ANDROID)
    ENDIF(PKG_CONFIG_FOUND)

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
            SET(SPEEX_LIBRARIES -lspeex)
        ENDIF(SPEEX_FOUND)
    ELSE(PKG_CONFIG_FOUND)
        SET(SPEEX_LIBRARIES -lspeex)
    ENDIF(PKG_CONFIG_FOUND)

    SET(RABBITIM_PACKAGE_REQUIRES "${RABBITIM_PACKAGE_REQUIRES},libspeex1,libspeexdsp1")
ENDIF(OPTION_RABBITIM_USE_SPEEX)

#增加检测ffmpeg库
OPTION(OPTION_RABBITIM_USER_FFMPEG "Use ffmpeg library" ON)
IF(OPTION_RABBITIM_USER_FFMPEG)
    SET(RABBITIM_USER_FFMPEG ON)
    SET(RABBITIM_PACKAGE_REQUIRES "${RABBITIM_PACKAGE_REQUIRES}, libavcodec-dev, libavformat-dev, libavutil-dev, libswscale-dev")

    ADD_DEFINITIONS("-D__STDC_CONSTANT_MACROS" "-DRABBITIM_USER_FFMPEG") #ffmpeg需要
    IF(PKG_CONFIG_FOUND)
        find_package(FFMPEG REQUIRED COMPONENTS avcodec avformat avutil swscale)
        IF(FFMPEG_FOUND)
            INCLUDE_DIRECTORIES("${FFMPEG_INCLUDE_DIRS}")
        ENDIF(FFMPEG_FOUND)
    ELSEIF(PKG_CONFIG_FOUND)
        #message("Must use pkg-config, please install it.")
        SET(FFMPEG_LIBRARIES -lavcodec -lavformat -lavutil -lswscale -lswresample -lx264 -lpthread)
    ENDIF(PKG_CONFIG_FOUND)

ENDIF(OPTION_RABBITIM_USER_FFMPEG)
MESSAGE("Use ffmpeg library:${RABBITIM_USER_FFMPEG}\n")

#TODO:增加检测opencv库
find_package(OpenCV)
OPTION(OPTION_RABBITIM_USER_OPENCV "Use opencv library" OFF)
IF(OPTION_RABBITIM_USER_OPENCV)
    add_definitions("-DRABBITIM_USER_OPENCV")
    SET(RABBITIM_USER_OPENCV ON)
    IF(WIN32)
        SET(OPENCV_VERSION 300)
    ENDIF(WIN32)
    SET(OPENCV_LIBS 
            opencv_imgproc${OPENCV_VERSION}
            opencv_videoio${OPENCV_VERSION}
            opencv_video${OPENCV_VERSION}
            opencv_core${OPENCV_VERSION}
            )
    IF(ANDROID)
        SET(OPENCV_LIBS ${OPENCV_LIBS}
                         opencv_androidcamera
                         opencv_imgcodecs 
                         opencv_info 
                         libjpeg
                )

        SET(ANDROID_EXTRA_LIBS #第三方信赖库
                "${CMAKE_SOURCE_DIR}/ThirdLibary/android/lib/libopencv_info.so,${CMAKE_SOURCE_DIR}/ThirdLibary/android/lib/libnative_camera_r4.2.0.so"  #TODO:修改成你手机平台对应的版本，如果没有，则取最近的版本
                )
    ELSE(ANDROID)
        SET(OPENCV_LIBS  ${OPENCV_LIBS} zlib)
        SET(RABBITIM_PACKAGE_REQUIRES "${RABBITIM_PACKAGE_REQUIRES}, libopencv-dev")
    ENDIF(ANDROID)
ELSEIF(OPTION_RABBITIM_USER_OPENCV)
    IF(ANDROID)
        message("android must opencv library, please install library, and -DOPTION_RABBITIM_USER_OPENCV=ON .")
    ENDIF(ANDROID)
ENDIF(OPTION_RABBITIM_USER_OPENCV)
MESSAGE("Use opencv library:${OPTION_RABBITIM_USER_OPENCV}\n")

#检测libcurl库
OPTION(OPTION_RABBITIM_USER_LIBCURL "Use curl library" ON)
IF(OPTION_RABBITIM_USER_LIBCURL)
    SET(RABBITIM_USER_LIBCURL ON)
    add_definitions("-DRABBITIM_USER_LIBCURL")
    SET(RABBITIM_PACKAGE_REQUIRES "${RABBITIM_PACKAGE_REQUIRES}, libcurl4-openssl-dev")

    IF(MSVC)
        SET(CURL_LIBRARIES -llibcurl)
    ELSE(MSVC)

        IF(PKG_CONFIG_FOUND)
            pkg_check_modules(CURL REQUIRED libcurl)
            IF(CURL_FOUND)
                add_compile_options(${CURL_CFLAGS})
            ELSE(CURL_FOUND)
                SET(CURL_LIBRARIES -lcurl -lssl -lcrypto -lz)
            ENDIF(CURL_FOUND)
        ELSE(PKG_CONFIG_FOUND)
            find_package(CURL REQUIRED)
            if(CURL_FOUND)
                include_directories(${CURL_INCLUDE_DIR})
            endif(CURL_FOUND)
        ENDIF(PKG_CONFIG_FOUND)

    ENDIF(MSVC)

    include_directories(${CURL_INCLUDE_DIRS})

ENDIF(OPTION_RABBITIM_USER_LIBCURL)
MESSAGE("Use curl library:${OPTION_RABBITIM_USER_LIBCURL}\n")

#检测openssl库
OPTION(OPTION_RABBITIM_USER_OPENSSL "Use openssl library" ON)
IF(OPTION_RABBITIM_USER_OPENSSL)
    SET(RABBITIM_USER_OPENSSL ON)
    ADD_DEFINITIONS("-DRABBITIM_USER_OPENSSL")
    SET(RABBITIM_PACKAGE_REQUIRES "${RABBITIM_PACKAGE_REQUIRES}, libssl-dev")

    IF(MSVC)
        SET(OPENSSL_LIBRARIES -llibeay32 -lssleay32)
    ELSE(MSVC)
 
       IF(PKG_CONFIG_FOUND)
            pkg_check_modules(OPENSSL REQUIRED openssl)
            IF(OPENSSL_FOUND)
                add_compile_options(${OPENSSL_CFLAGS})
            ELSE()
                SET(OPENSSL_LIBRARIES -lcrypto -lssl)
            ENDIF(OPENSSL_FOUND)
        ELSE(PKG_CONFIG_FOUND)
            find_package(OpenSSL REQUIRED)
            if(OPENSSL_FOUND)
                include_directories(${OPENSSL_INCLUDE_DIR})
            endif(OPENSSL_FOUND)
        ENDIF(PKG_CONFIG_FOUND)

    ENDIF(MSVC)

ENDIF(OPTION_RABBITIM_USER_OPENSSL)
MESSAGE("Use openssl library:${OPTION_RABBITIM_USER_OPENSSL}\n")

#qxmpp库
#连接静态QXMPP库时，必须加上-DQXMPP_STATIC。生成静态QXMPP库时，qmake 需要加上 QXMPP_LIBRARY_TYPE=staticlib 参数
ADD_DEFINITIONS("-DQXMPP") # -DQXMPP_STATIC")
IF(PKG_CONFIG_FOUND)
    pkg_check_modules(QXMPP REQUIRED qxmpp)
    IF(QXMPP_FOUND)
            add_compile_options(${QXMPP_CFLAGS})
    ELSE(QXMPP_FOUND)
            SET(QXMPP_LIBRARIES -lqxmpp)
    ENDIF(QXMPP_FOUND)
ELSE(PKG_CONFIG_FOUND)
    SET(QXMPP_LIBRARIES -lqxmpp)
ENDIF(PKG_CONFIG_FOUND)

SET(RABBITIM_LIBS
    ${RABBITIM_LIBS}
    ${QXMPP_LIBRARIES}
    ${OPENCV_LIBS}
    ${FFMPEG_LIBRARIES}
    ${VPX_LIBRARIES}
    ${SPEEX_LIBRARIES}
    ${CURL_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    Qt5::Widgets
    Qt5::Network
    Qt5::Multimedia
    Qt5::Xml
    Qt5::Core)

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
    SET(RABBITIM_LIBS
        ${RABBITIM_LIBS} 
        Qt5::AndroidExtras)
    qt5_wrap_ui(RABBITIM_UIS ${RABBITIM_UIS})
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
IF(Qt5WebKitWidgets_FOUND)
    SET(RABBITIM_LIBS
        ${RABBITIM_LIBS} 
        Qt5::WebKitWidgets)
ENDIF(Qt5WebKitWidgets_FOUND)
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
    IF(WIN32)
        IF(MINGW)
            #windeployqt 分发时，是根据是否 strip 来判断是否是 DEBUG 版本
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
        SET(QT_LIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::Multimedia Qt5::Xml)
        SET(QT_PLUGINS Qt5Core_PLUGINS 
                           Qt5Gui_PLUGINS
                           Qt5Widgets_PLUGINS
                           Qt5Network_PLUGINS
                           Qt5Multimedia_PLUGINS
                           Qt5Xml_PLUGINS)
        IF(Qt5WebKitWidgets_FOUND)
			SET(QT_LIBS ${QT_LIBS} Qt5::WebKitWidgets)
			SET(QT_PLUGINS ${QT_PLUGINS} Qt5WebKitWidgets_PLUGINS)
		ENDIF(Qt5WebKitWidgets_FOUND)
        INSTALL_QT_LIBRARYS(${QT_LIBS})
        INSTALL_QT_PLUGINS(${QT_PLUGINS})
    ENDIF(WIN32)
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
    FILE(GLOB THIRD_DLL_LIBRARY "${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/bin/*.dll")
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
MESSAGE("THIRD_DLL_LIBRARY:${THIRD_DLL_LIBRARY}")
FOREACH(file ${THIRD_DLL_LIBRARY})
    IF(EXISTS "${file}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${file}" "${PROJECT_BINARY_DIR}/."
            )
     ENDIF(EXISTS "${file}")
ENDFOREACH(file)

#安装5:安装相关文件
install(FILES License.md Authors.txt ChangeLog.md DESTINATION .)
