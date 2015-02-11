cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(RabbitIm)

# 建立要删除的文件
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
# 建立删除目标
ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")

SET(INSTALL_DIRECTORIES ${CMAKE_INSTALL_PREFIX}/RabbitIm)

# Find Git Version Patch
FIND_PROGRAM(GIT git)
IF(GIT)
    EXECUTE_PROCESS(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${GIT} rev-parse --short HEAD
        OUTPUT_VARIABLE BUILD_VERSION  OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    MESSAGE("Git Version: ${BUILD_VERSION}\n")
ENDIF(GIT)

MESSAGE("\n")

#增加pkgconfig模块
#include(FindPkgConfig)

#TODO:判断 qt 版本

FIND_PACKAGE(Qt5 CONFIG REQUIRED Gui Core Network Xml Multimedia Widgets LinguistTools)
SET(QT_INSTALL_DIR "${Qt5_DIR}/../../..")

include(cmake/RabbitImUtils.cmake)

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_CXX_FLAGS "-std=c++0x") #启用C++11

# Add definitions for static/style library 
OPTION(STATIC "Build statically" OFF)
MESSAGE("Build statically: ${STATIC}")
IF(STATIC)
    ADD_DEFINITIONS(-DQT_STATIC)
ELSE(STATIC)
    ADD_DEFINITIONS(-DQT_SHARED)
ENDIF(STATIC)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    ADD_DEFINITIONS(-DDEBUG)# -DDEBUG_VIDEO_TIME )
ENDIF()

#设置头文件包含位置 
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
IF(MSVC)
    FIND_PACKAGE(Qt5 CONFIG REQUIRED WebKitWidgets)
    SET(RABBITIM_SYSTEM windows)
    SET(TOOLCHAIN_NAME windows_msvc)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmtd /NODEFAULTLIB:libcmt")
ELSEIF(MINGW)
    FIND_PACKAGE(Qt5 CONFIG REQUIRED WebKitWidgets)
    SET(RABBITIM_SYSTEM windows)
    SET(TOOLCHAIN_NAME windows_mingw)
    # Windows compatibility
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
    # Statically link with libgcc
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ ")
ELSEIF(ANDROID)
    SET(RABBITIM_SYSTEM android)
    SET(TOOLCHAIN_NAME android)
    ADD_DEFINITIONS(-DMOBILE)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/android/jni)
    FIND_PACKAGE(Qt5 CONFIG REQUIRED  AndroidExtras)
    #SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wno-psabi -march=armv7-a -mfloat-abi=softfp -mfpu=vfp -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums  -Wa,--noexecstack -gdwarf-2 -marm -fno-omit-frame-pointer -Wall -Wno-psabi -W -fPIE)
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined -Wl,-z,noexecstack -shared --sysroot=${ANDROID_SYSROOT}")
ELSE(LINUX OR UNIX)
    FIND_PACKAGE(Qt5 CONFIG REQUIRED WebKitWidgets)
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
SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME})

# 依赖的第三方库
OPTION(OPTION_USE_VPX "Use vpx library" ON)
MESSAGE("Use vpx library:${USE_VPX}\n")
IF(OPTION_USE_VPX)
    SET(USE_VPX ON)
    SET(VPX_LIBS vpx)
    IF(ANDROID)
        SET(VPX_LIBS ${VPX_LIBS} cpu-features)
    ENDIF(ANDROID)
ENDIF(OPTION_USE_VPX)

OPTION(USE_SPEEX "Use speex library" OFF)
MESSAGE("Use speex library:${USE_SPEEX}\n")
IF(USE_SPEEX)
    SET(SPEEX_LIBS speex)
ENDIF()

#TODO:增加检测ffmpeg库
OPTION(OPTION_RABBITIM_USER_FFMPEG "Use ffmpeg library" ON)
IF(OPTION_RABBITIM_USER_FFMPEG)
    SET(RABBITIM_USER_FFMPEG ON)
    IF(RABBITIM_USER_FFMPEG)
        ADD_DEFINITIONS("-D__STDC_CONSTANT_MACROS" "-DRABBITIM_USER_FFMPEG") #ffmpeg需要
        IF(MSVC)
            SET(FFMPEG_LIBS 
                ${THIRD_LIB_DIR}/libavcodec.a 
                ${THIRD_LIB_DIR}/libavformat.a 
                ${THIRD_LIB_DIR}/libswscale.a 
                ${THIRD_LIB_DIR}/libavfilter.a 
                ${THIRD_LIB_DIR}/libavutil.a)
        ELSE()
            SET(FFMPEG_LIBS avcodec avformat swscale avfilter avutil)
        ENDIF()
    ENDIF(RABBITIM_USER_FFMPEG)
ENDIF(OPTION_RABBITIM_USER_FFMPEG)
MESSAGE("Use ffmpeg library:${RABBITIM_USER_FFMPEG}\n")

#TODO:增加检测opencv库
OPTION(OPTION_RABBITIM_USER_OPENCV "Use opencv library" OFF)
IF(OPTION_RABBITIM_USER_OPENCV)
    IF(RABBITIM_USER_OPENCV)
        add_definitions("-DRABBITIM_USER_OPENCV")
        SET(RABBITIM_USER_OPENCV ON)
        IF(WIN32)
            SET(OPENCV_VERSION 300)
        ENDIF(WIN32)
        SET(OPENCV_LIBS 
            opencv_core${OPENCV_VERSION}
            opencv_imgproc${OPENCV_VERSION})
    ENDIF(RABBITIM_USER_OPENCV)
ENDIF(OPTION_RABBITIM_USER_OPENCV)
MESSAGE("Use opencv library:${RABBITIM_USER_OPENCV}\n")

#检测libcurl库
OPTION(OPTIOIN_RABBITIM_USER_LIBCURL "Use curl library" ON)
IF(OPTIOIN_RABBITIM_USER_LIBCURL)
    find_package(CURL)
    if(CURL_FOUND)
        add_definitions("-DRABBITIM_USER_LIBCURL")
        SET(RABBITIM_USER_LIBCURL ON)
        include_directories(${CURL_INCLUDE_DIR})
        SET(LIBCURL_LIBS ${CURL_LIBRARY})
    else(CURL_FOUND)
        MESSAGE("Don't find libcurl library\n")
    endif(CURL_FOUND)
ENDIF(OPTIOIN_RABBITIM_USER_LIBCURL)
MESSAGE("Use curl library:${RABBITIM_USER_LIBCURL}\n")

#检测openssl库
OPTION(OPTION_RABBITIM_USER_OPENSSL "Use openssl library" ON)
IF(OPTION_RABBITIM_USER_OPENSSL)
    find_package(OpenSSL)
    if(OPENSSL_FOUND)
        add_definitions("-DRABBITIM_USER_OPENSSL")
        SET(RABBITIM_USER_OPENSSL ON)
        include_directories(${OPENSSL_INCLUDE_DIR})
        SET(OPENSSL_LIBS ${OPENSSL_LIBRARIES})
    else(OPENSSL_FOUND)
        MESSAGE("Don't find openssl library\n")
    endif(OPENSSL_FOUND)
ENDIF(RABBITIM_USER_OPENSSL)
MESSAGE("Use openssl library:${RABBITIM_USER_OPENSSL}\n")

#连接静态QXMPP库时，必须加上-DQXMPP_STATIC。生成静态QXMPP库时，qmake 需要加上 QXMPP_LIBRARY_TYPE=staticlib 参数
ADD_DEFINITIONS("-DQXMPP") # -DQXMPP_STATIC")
IF(WIN32 AND MSVC AND CMAKE_BUILD_TYPE MATCHES Debug)
    SET(QXMPP_LIBS qxmpp_d0)
ELSE()
    SET(QXMPP_LIBS qxmpp0)
ENDIF()
message("QXMPP_LIBS:${QXMPP_LIBS}")

SET(RABBITIM_LIBS
    ${RABBITIM_LIBS}
    ${QXMPP_LIBS}
    ${OPENCV_LIBS}
    ${FFMPEG_LIBS}
    ${VPX_LIBS}
    ${SPEEX_LIBS}
    ${LIBCURL_LIBS}
    ${OPENSSL_LIBS}
    Qt5::Widgets
    Qt5::Network
    Qt5::Multimedia
    Qt5::Xml
    Qt5::Core)

include(cmake/install_version.cmake)
include(cmake/cmake_build_file.cmake)

SET(RABBITIM_RCC_FILES Resource/Resource.qrc Resource/sink/dark/style.qrc  AppIcon.rc)

#生成目标
IF(ANDROID)
    add_library(${PROJECT_NAME} SHARED 
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        )
    SET(RABBITIM_LIBS
        ${RABBITIM_LIBS} 
        Qt5::AndroidExtras)
ELSEIF(WIN32)
    add_executable(${PROJECT_NAME} WIN32
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        ${RABBITIM_UIS}
        )
    SET(RABBITIM_LIBS
        ${RABBITIM_LIBS} 
        Qt5::WebKitWidgets)
ELSE()
    add_executable(${PROJECT_NAME} 
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        ${RABBITIM_UIS}
        )
    SET(RABBITIM_LIBS
        ${RABBITIM_LIBS} 
        Qt5::WebKitWidgets)
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
        add_dependencies(${PROJECT_NAME}  translations)
        foreach(_file ${QM_FILES})
            IF(ANDROID)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E make_directory  ${PROJECT_BINARY_DIR}/android-build/translations
                        COMMAND ${CMAKE_COMMAND} -E copy ${_file}  ${PROJECT_BINARY_DIR}/android-build/translations
                        COMMAND ${CMAKE_COMMAND} -E copy  ${QT_INSTALL_DIR}/translations/qt_zh_CN.qm   ${PROJECT_BINARY_DIR}/android-build/translations
                        )
            ELSE(ANDROID)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E make_directory translations
                        COMMAND ${CMAKE_COMMAND} -E copy ${_file} translations
                        )
            ENDIF(ANDROID)
        endforeach()
        FILE(COPY ${QT_INSTALL_DIR}/translations/qt_zh_CN.qm DESTINATION "translations")
        #FILE(COPY ${QM_FILES} DESTINATION "translations")
    ENDIF(NOT Qt5_LRELEASE_EXECUTABLE)
ENDIF(RABBITIM_TRANSLATIONS)

GENERATED_DEPLOYMENT_SETTINGS()
find_program(ANT NAMES ant) # PATHS  "/usr/bin")
message("ant:${ANT}\n")

#只发行版本才安装
IF(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    IF(ANDROID)
        INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}")
    ELSE()
        #设置安装目录
        #SET(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")
        INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${INSTALL_DIRECTORIES}")
    ENDIF()
    INSTALL(FILES ${QT_INSTALL_DIR}/translations/qt_zh_CN.qm DESTINATION "${INSTALL_DIRECTORIES}/translations")
    INSTALL(FILES ${QM_FILES} DESTINATION "${INSTALL_DIRECTORIES}/translations")
    IF(WIN32)
        add_custom_target(Deployment DEPENDS ${PROJECT_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.exe" "${INSTALL_DIRECTORIES}/."
            COMMAND "${QT_INSTALL_DIR}/bin/windeployqt" 
                --compiler-runtime
                --no-translations
                --verbose 7
                "${INSTALL_DIRECTORIES}/${PROJECT_NAME}.exe"
        )
    ELSE(WIN32)
        SET(QT_LIBS Qt5::Widgets Qt5::Network Qt5::Multimedia Qt5::Core Qt5::Gui Qt5::Xml Qt5::WebKitWidgets)
        INSTALL_QT_LIBRARYS(${QT_LIBS})
        INSTALL_QT_PLUGINS(Qt5Gui_PLUGINS Qt5Network_PLUGINS Qt5Widgets_PLUGINS Qt5Xml_PLUGINS)
    ENDIF(WIN32)
ENDIF(NOT CMAKE_BUILD_TYPE MATCHES Debug)

IF(ANDROID)
    add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/libRabbitIm.so ${PROJECT_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/libRabbitIm.so
        COMMAND "${QT_INSTALL_DIR}/bin/androiddeployqt" 
            --input ${PROJECT_BINARY_DIR}/android-libRabbitIm.so-deployment-settings.json 
            --output ${PROJECT_BINARY_DIR}/android-build 
            --verbose
            #--ant /usr/bin/ant 
            #--jdk $ENV{JAVA_HOME}
    )
ENDIF(ANDROID)

#复制第三方依赖库的动态库到编译目录下
IF(WIN32)
    IF(EXISTS ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/bin/*.dll)
        add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/bin/*.dll ${PROJECT_BINARY_DIR}/.
        )
    ENDIF()
ENDIF(WIN32)
