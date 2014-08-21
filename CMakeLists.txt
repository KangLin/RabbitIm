cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(RabbitIm)

#增加pkgconfig模块
#include(FindPkgConfig)

FIND_PACKAGE(Qt5 CONFIG REQUIRED  core gui network xml multimedia widgets)

#打开 qt 编译工具  
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#设置头文件包含位置  
set(CMAKE_INCLUDE_CURRENT_DIR ON)
IF(MSVC)
    set(TOOLCHAIN windows_msvc)
ELSEIF(MINGW)
    set(TOOLCHAIN windows_mingw)
ENDIF()
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} 
    ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN}/include)

#add_subdirectory(Widgets)

set(RABBITIM_SOURCES
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
        
set(RABBITIM_RCC_FILES Resource/Resource.qrc)

add_executable(${PROJECT_NAME}
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        )
target_link_libraries(${PROJECT_NAME} 
        Qt5::Widgets
        Qt5::Network
        Qt5::Multimedia
        Qt5::Xml
        Qt5::Core)
