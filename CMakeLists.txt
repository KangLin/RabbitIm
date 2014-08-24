cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(RabbitIm)

#增加pkgconfig模块
#include(FindPkgConfig)

FIND_PACKAGE(Qt5 CONFIG REQUIRED  Core Gui Network Xml Multimedia Widgets)

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

SET(CMAKE_VERBOSE_MAKEFILE ON)

ADD_DEFINITIONS("-D__STDC_CONSTANT_MACROS") #ffmpeg需要

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

LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN}/lib)

SET(FFMPEG_LIBS avcodec avformat swscale avfilter avutil)
SET(RABBITIM_LIBS 
    ${FFMPEG_LIBS}
    qxmpp
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
