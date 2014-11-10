cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(RabbitIm)

# 建立要删除的文件
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
# 建立删除目标
ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")

# Find Git Version Patch
FIND_PROGRAM(GIT git)
IF(GIT)
    EXECUTE_PROCESS(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${GIT} rev-parse --short HEAD
        OUTPUT_VARIABLE GIT_OUT OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    SET(PROJECT_VERSION_PATCH "${GIT_OUT}")
    MESSAGE("Git Version Patch: ${GIT_OUT}")
ELSE(GIT)
    IF(GITBUILD)
        SET(PROJECT_VERSION_PATCH "${GITBUILD}")
    ELSE(GITBUILD)
        SET(GIT_OUT 0)
    ENDIF(GITBUILD)
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
SET(CMAKE_CXX_FLAGS "-std=c++11") #启用C++11

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
    SET(TOOLCHAIN_NAME windows_msvc)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmtd /NODEFAULTLIB:libcmt")
ELSEIF(MINGW)
    SET(TOOLCHAIN_NAME windows_mingw)
    # Windows compatibility
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
    # Statically link with libgcc
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ ")
ELSEIF(ANDROID)
    SET(TOOLCHAIN_NAME android)
    ADD_DEFINITIONS(-DMOBILE)
    #SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wno-psabi -march=armv7-a -mfloat-abi=softfp -mfpu=vfp -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums  -Wa,--noexecstack -gdwarf-2 -marm -fno-omit-frame-pointer -Wall -Wno-psabi -W -fPIE)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined -Wl,-z,noexecstack -shared")
ELSE(LINUX OR UNIX)
    ADD_DEFINITIONS(-DUNIX)
    SET(TOOLCHAIN_NAME unix)
ENDIF()
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} 
    ${CMAKE_SOURCE_DIR}/Widgets/FrmCustom
    ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/include)

SET(THIRD_LIB_DIR ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/lib)
LINK_DIRECTORIES(${THIRD_LIB_DIR})

# 依赖的第三方库
OPTION(USE_VPX "Use vpx library" ON)
MESSAGE("Use vpx library:${USE_VPX}\n")
IF(USE_VPX)
    SET(VPX_LIBS vpx)
    IF(ANDROID)
        SET(VPX_LIBS ${VPX_LIBS} cpu-features)
    ENDIF(ANDROID)
ENDIF(USE_VPX)

OPTION(USE_SPEEX "Use speex library" OFF)
MESSAGE("Use speex library:${USE_SPEEX}\n")
IF(USE_SPEEX)
    SET(SPEEX_LIBS speex)
ENDIF()

OPTION(RABBITIM_USER_FFMPEG "Use ffmpeg library" ON)
MESSAGE("Use ffmpeg library:${RABBITIM_USER_FFMPEG}\n")
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

OPTION(RABBITIM_USER_OPENCV "Use opencv library" OFF)
MESSAGE("Use opencv library:${RABBITIM_USER_OPENCV}\n")
IF(RABBITIM_USER_OPENCV)
    add_definitions("-DRABBITIM_USER_OPENCV")
    IF(WIN32)
        SET(OPENCV_VERSION 300)
    ENDIF(WIN32)
    SET(OPENCV_LIBS 
        opencv_core${OPENCV_VERSION}
        opencv_imgproc${OPENCV_VERSION})
ENDIF(RABBITIM_USER_OPENCV)

#连接静态QXMPP库时，必须加上-DQXMPP_STATIC。生成静态QXMPP库时，qmake 需要加上 QXMPP_LIBRARY_TYPE=staticlib 参数
ADD_DEFINITIONS("-DQXMPP -DQXMPP_STATIC")
IF(WIN32 AND MSVC AND CMAKE_BUILD_TYPE MATCHES Debug)
    SET(QXMPP_LIBS qxmpp_d)
ELSE()
    SET(QXMPP_LIBS qxmpp)
ENDIF()
message("QXMPP_LIBS:${QXMPP_LIBS}")
SET(RABBITIM_LIBS
    ${QXMPP_LIBS}
    ${OPENCV_LIBS}
    ${FFMPEG_LIBS}
    ${VPX_LIBS}
    ${SPEEX_LIBS}
    )

include(cmake/cmake_build_file.cmake)

SET(RABBITIM_RCC_FILES Resource/Resource.qrc  AppIcon.rc)

#生成目标
IF(ANDROID)
    add_library(${PROJECT_NAME} SHARED 
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        )
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
target_link_libraries(${PROJECT_NAME} 
        ${RABBITIM_LIBS}
        Qt5::Widgets
        Qt5::Network
        Qt5::Multimedia
        Qt5::Xml
        Qt5::Core)

#翻译
SET(TS_FILES Resource/translations/app_zh_CN.ts)
OPTION(REFRESH_TRANSLATIONS "Refresh translations on compile" ON)
MESSAGE("Refresh translations on compile: ${REFRESH_TRANSLATIONS}\n")
IF(REFRESH_TRANSLATIONS)
    IF(NOT Qt5_LRELEASE_EXECUTABLE)
        MESSAGE(WARNING "Could not find lrelease. Your build won't contain translations.")
    ELSE(NOT Qt5_LRELEASE_EXECUTABLE)
        qt5_add_translation(QM_FILES ${TS_FILES})
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
ENDIF(REFRESH_TRANSLATIONS)

GENERATED_DEPLOYMENT_SETTINGS()
find_program(ANT NAMES ant PATHS  "/usr/bin")
message("ant:${ANT}\n")

#只发行版本才安装
IF(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    IF(ANDROID)
        INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}")
    ELSE()
        #设置安装目录
        SET(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")
        INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}")
    ENDIF()
    INSTALL(FILES ${QT_INSTALL_DIR}/translations/qt_zh_CN.qm DESTINATION "${CMAKE_INSTALL_PREFIX}/translations")
    INSTALL(FILES ${QM_FILES} DESTINATION "${CMAKE_INSTALL_PREFIX}/translations")
    SET(QT_LIBS Qt5::Widgets Qt5::Network Qt5::Multimedia Qt5::Core Qt5::Gui Qt5::Xml)
    INSTALL_QT_LIBRARYS(${QT_LIBS})
    INSTALL_QT_PLUGINS(Qt5Gui_PLUGINS Qt5Network_PLUGINS Qt5Widgets_PLUGINS Qt5Xml_PLUGINS)
    IF(WIN32)
        INSTALL(FILES ${QT_INSTALL_DIR}/bin/icudt52.dll DESTINATION "${CMAKE_INSTALL_PREFIX}")
        INSTALL(FILES ${QT_INSTALL_DIR}/bin/icuin52.dll DESTINATION "${CMAKE_INSTALL_PREFIX}")
        INSTALL(FILES ${QT_INSTALL_DIR}/bin/icuuc52.dll DESTINATION "${CMAKE_INSTALL_PREFIX}")
    ENDIF(WIN32)
ENDIF(NOT CMAKE_BUILD_TYPE MATCHES Debug)

IF(ANDROID)
    add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY_OUTPUT_PATH}/libRabbitIm.so ${PROJECT_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/libRabbitIm.so
        COMMAND "${QT_INSTALL_DIR}/bin/androiddeployqt" 
            --input ${PROJECT_BINARY_DIR}/android-libRabbitIm.so-deployment-settings.json 
            --output ${PROJECT_BINARY_DIR}/android-build 
            --verbose
            #--ant /usr/bin/ant 
            #--jdk $ENV{JAVA_HOME}
    )
ENDIF(ANDROID)
