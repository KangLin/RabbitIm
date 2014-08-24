cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

include(cmake/RabbitImUtils.cmake)

ADD_CUSTOM_TARGET(uninstall 
    "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake") 

#TODO:判断 qt 版本

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

project(RabbitIm)

#增加pkgconfig模块
#include(FindPkgConfig)

FIND_PACKAGE(Qt5 CONFIG REQUIRED  Core Gui Network Xml Multimedia Widgets LinguistTools)

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

SET(CMAKE_VERBOSE_MAKEFILE ON)

# Add definitions for static/style library 
OPTION(STATIC "Build statically" OFF)
MESSAGE("Build statically: ${STATIC}")
IF(STATIC)
    ADD_DEFINITIONS(-DQT_STATIC)
ELSE(STATIC)
    ADD_DEFINITIONS(-DQT_SHARED)
ENDIF(STATIC)

IF(MINGW) 
    # Windows compatibility
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
    # Statically link with libgcc
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ ")
ELSEIF(MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmtd /NODEFAULTLIB:libcmt")
ENDIF(MINGW)

#设置头文件包含位置 
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
IF(MSVC)
    SET(TOOLCHAIN windows_msvc)
ELSEIF(MINGW)
    SET(TOOLCHAIN windows_mingw)
ELSEIF(ANDROID)
    SET(TOOLCHAIN android)
ELSE(LINUX OR UNIX)
    SET(TOOLCHAIN unix)
ENDIF()
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} 
    ${CMAKE_SOURCE_DIR}/Widgets/FrmCustom
    ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN}/include)

SET(THIRD_LIB_DIR ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN}/lib)
LINK_DIRECTORIES(${THIRD_LIB_DIR})

OPTION(USE_VPX "Use vpx library" ON)
MESSAGE("Use vpx library:${USE_VPX}\n")
IF(USE_VPX)
    SET(VPX_LIBS vpx)
ENDIF(USE_VPX)

OPTION(USE_SPEEX "Use speex library" OFF)
MESSAGE("Use speex library:${USE_SPEEX}\n")
IF(USE_SPEEX)
    SET(SPEEX_LIBS speex)
ENDIF()

OPTION(USE_FFMPEG "Use ffmpeg library" ON)
MESSAGE("Use ffmpeg library:${USE_FFMPEG}\n")
IF(USE_FFMPEG)
    ADD_DEFINITIONS("-D__STDC_CONSTANT_MACROS") #ffmpeg需要
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
ENDIF(USE_FFMPEG)

OPTION(USE_OPENCV "Use opencv library" OFF)
MESSAGE("Use opencv library:${USE_OPENCV}\n")
IF(USE_OPENCV)
    add_definitions("-DRABBITIM_USER_OPENCV")
    IF(WIN32)
        SET(OPENCV_VERSION 300)
    ENDIF(WIN32)
    SET(OPENCV_LIBS 
        opencv_core${OPENCV_VERSION}
        opencv_imgproc${OPENCV_VERSION})
ENDIF(USE_OPENCV)

#连接静态QXMPP库时，必须加上-DQXMPP_STATIC。生成静态QXMPP库时，qmake 需要加上 QXMPP_LIBRARY_TYPE=staticlib 参数
ADD_DEFINITIONS("-DQXMPP_STATIC")
IF(CMAKE_BUILD_TYPE MATCHES Debug)
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

#add_subdirectory(Widgets)

SET(RABBITIM_SOURCES
    main.cpp
    MainWindow.cpp 
    XmppClient.cpp 
    Global.cpp 
    Tool.cpp 
    Widgets/FrmSendFile/DlgSendManage.cpp 
    Widgets/FrmSendFile/FrmSendFile.cpp 
    Widgets/FrmLogin/FrmLogin.cpp 
    Widgets/FrmUserList/FrmUserList.cpp 
    Widgets/FrmUserList/Roster.cpp 
    Widgets/FrmUserList/GroupItem.cpp 
    Widgets/FrmMessage/FrmMessage.cpp 
    Widgets/FrmMessage/Message.cpp 
    Widgets/FrmRegister/FrmRegister.cpp 
    Widgets/FrmAbout/FrmAbout.cpp 
    Widgets/FrmVideo/FrmVideo.cpp 
    Widgets/FrmVideo/CaptureVideoFrame.cpp 
    Widgets/FrmVideo/FrmPlayer.cpp 
    Widgets/FrmVideo/DataVideoBuffer.cpp 
    Widgets/FrmVideo/FrameProcess.cpp 
    Widgets/FrmAddRoster/FrmAddRoster.cpp 
    Widgets/FrmCustom/CustomTreeView.cpp 
    Widgets/FrmVideo/RecordAudio.cpp 
    Widgets/FrmVideo/CCamera.cpp 
    Widgets/FrmLogin/FrmLoginSettings.cpp 
    Widgets/FrmOptions/FrmOptions.cpp 
    Widgets/FrmUservCard/FrmUservCard.cpp 
    Widgets/FrmCustom/Label.cpp  
    Widgets/FrmGroupChat/FrmGroupChatList.cpp 
    Widgets/FrmGroupChat/FrmGroupChatFind.cpp 
    Widgets/FrmGroupChat/Conference.cpp 
    Widgets/FrmGroupChat/FrmGroupChat.cpp 
    Widgets/FrmGroupChat/FrmCreateGroupChatRoom.cpp 
    Widgets/FrmMain/FrmMain.cpp 
    Widgets/FrmCustom/MenuMessageSend.cpp
)

if(WIN32)
    SET(RABBITIM_SOURCES 
        ${RABBITIM_SOURCES}
        Widgets/DlgScreenShot/DlgScreenShot.cpp)
endif(WIN32)

SET(RABBITIM_RCC_FILES Resource/Resource.qrc)
SET(TS_FILES Resource/translations/app_zh_CN.ts)

#翻译
OPTION(REFRESH_TRANSLATIONS "Refresh translations on compile" ON)
MESSAGE("Refresh translations on compile: ${REFRESH_TRANSLATIONS}\n")
IF(REFRESH_TRANSLATIONS)
    IF(NOT Qt5_LRELEASE_EXECUTABLE)
        MESSAGE(WARNING "Could not find lrelease. Your build won't contain translations.")
    ELSE(NOT Qt5_LRELEASE_EXECUTABLE)
        qt5_add_translation(QM_FILES ${TS_FILES})
        ADD_CUSTOM_TARGET(translations ALL DEPENDS ${QM_FILES})
        FILE(COPY ${QM_FILES} DESTINATION "translations")
    ENDIF(NOT Qt5_LRELEASE_EXECUTABLE)
ENDIF(REFRESH_TRANSLATIONS)

add_executable(${PROJECT_NAME}
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        )

target_link_libraries(${PROJECT_NAME} 
        ${RABBITIM_LIBS}
        Qt5::Widgets
        Qt5::Network
        Qt5::Multimedia
        Qt5::Xml
        Qt5::Core)

SET(QT_INSTALL_DIR "${_qt5Core_install_prefix}")

INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}")
INSTALL_QT_LIBRARY(Widgets)
INSTALL_QT_LIBRARY(NetWork)
INSTALL_QT_LIBRARY(Multimedia)
INSTALL_QT_LIBRARY(Core)
INSTALL_QT_LIBRARY(Xml)
INSTALL_QT_LIBRARY(Gui)
IF(WIN32)
    INSTALL(FILES ${QT_INSTALL_DIR}/bin/icudt52.dll DESTINATION "${CMAKE_INSTALL_PREFIX}")
    INSTALL(FILES ${QT_INSTALL_DIR}/bin/icuin52.dll DESTINATION "${CMAKE_INSTALL_PREFIX}")
    INSTALL(FILES ${QT_INSTALL_DIR}/bin/icuuc52.dll DESTINATION "${CMAKE_INSTALL_PREFIX}")
ELSE()
    INSTALL(FILES ${QT_INSTALL_DIR}/bin/icudt52.so DESTINATION "${CMAKE_INSTALL_PREFIX}")
    INSTALL(FILES ${QT_INSTALL_DIR}/bin/icuin52.so DESTINATION "${CMAKE_INSTALL_PREFIX}")
    INSTALL(FILES ${QT_INSTALL_DIR}/bin/icuuc52.so DESTINATION "${CMAKE_INSTALL_PREFIX}")
ENDIF()
INSTALL(FILES ${QT_INSTALL_DIR}/translations/qt_zh_CN.qm DESTINATION "${CMAKE_INSTALL_PREFIX}/translations")
INSTALL(FILES ${QM_FILES} DESTINATION "${CMAKE_INSTALL_PREFIX}/translations")
