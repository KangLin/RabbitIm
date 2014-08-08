SOURCES += main.cpp\
    MainWindow.cpp \
    XmppClient.cpp \
    Global.cpp \
    Tool.cpp \
    Widgets/FrmSendFile/DlgSendManage.cpp \
    Widgets/FrmSendFile/FrmSendFile.cpp \
    Widgets/DlgLogin/DlgLogin.cpp \
    Widgets/DlgLogin/FrmLogin.cpp \
    Widgets/FrmUserList/FrmUserList.cpp \
    Widgets/FrmUserList/Roster.cpp \
    Widgets/FrmUserList/GroupItem.cpp \
    Widgets/FrmMessage/FrmMessage.cpp \
    Widgets/FrmMessage/Message.cpp \
    Widgets/FrmRegister/FrmRegister.cpp \
    Widgets/FrmAbout/FrmAbout.cpp \
    Widgets/FrmVideo/FrmVideo.cpp \
    Widgets/FrmVideo/CaptureVideoFrame.cpp \
    Widgets/FrmVideo/FrmPlayer.cpp \
    Widgets/FrmVideo/DataVideoBuffer.cpp \
    Widgets/FrmVideo/FrameProcess.cpp \
    Widgets/FrmAddRoster/FrmAddRoster.cpp \
    Widgets/FrmCustom/CustomTreeView.cpp \
    Widgets/FrmVideo/RecordAudio.cpp \
    Widgets/FrmVideo/CCamera.cpp \
    Widgets/DlgLogin/FrmLoginSettings.cpp \
    Widgets/FrmOptions/FrmOptions.cpp \
    Widgets/FrmUservCard/FrmUservCard.cpp \
    Widgets/FrmCustom/Label.cpp \ 
    Widgets/WdgTableMain/WdgTableMain.cpp \
    Widgets/FrmGroupChat/FrmGroupChatList.cpp \
    Widgets/FrmGroupChat/FrmGroupChatFind.cpp \
    Widgets/FrmGroupChat/Conference.cpp \
    Widgets/FrmGroupChat/FrmGroupChat.cpp

HEADERS  += MainWindow.h \
    XmppClient.h \
    Global.h \
    Tool.h \
    Widgets/FrmSendFile/DlgSendManage.h \
    Widgets/FrmSendFile/FrmSendFile.h \
    Widgets/DlgLogin/DlgLogin.h \
    Widgets/DlgLogin/FrmLogin.h \
    Widgets/FrmUserList/FrmUserList.h \
    Widgets/FrmUserList/Roster.h \
    Widgets/FrmUserList/GroupItem.h \
    Widgets/FrmMessage/FrmMessage.h \
    Widgets/FrmMessage/Message.h \
    Widgets/FrmRegister/FrmRegister.h \
    Widgets/FrmAbout/FrmAbout.h \
    Widgets/FrmVideo/FrmVideo.h \
    Widgets/FrmVideo/CaptureVideoFrame.h \
    Widgets/FrmVideo/FrmPlayer.h \    
    Widgets/FrmVideo/DataVideoBuffer.h \
    Widgets/FrmVideo/FrameProcess.h \
    Widgets/FrmAddRoster/FrmAddRoster.h \
    Widgets/FrmCustom/CustomTreeView.h \
    Widgets/FrmVideo/RecordAudio.h \
    Widgets/FrmAbout/Version.h \
    Widgets/FrmVideo/CCamera.h \
    Widgets/DlgLogin/FrmLoginSettings.h \
    Widgets/FrmOptions/FrmOptions.h \
    Widgets/FrmUservCard/FrmUservCard.h \ 
    Widgets/FrmCustom/Label.h \ 
    Widgets/WdgTableMain/WdgTableMain.h \
    Widgets/FrmGroupChat/FrmGroupChatList.h \
    Widgets/FrmGroupChat/FrmGroupChatFind.h \
    Widgets/FrmGroupChat/Conference.h \
    Widgets/FrmGroupChat/FrmGroupChat.h

FORMS    += MainWindow.ui \
    Widgets/FrmSendFile/DlgSendManage.ui \
    Widgets/FrmSendFile/FrmSendFile.ui \
    Widgets/DlgLogin/DlgLogin.ui \
    Widgets/DlgLogin/FrmLogin.ui \
    Widgets/FrmUserList/FrmUserList.ui \
    Widgets/FrmMessage/FrmMessage.ui \
    Widgets/FrmRegister/FrmRegister.ui \
    Widgets/FrmAbout/FrmAbout.ui \
    Widgets/FrmVideo/FrmVideo.ui \
    Widgets/FrmAddRoster/FrmAddRoster.ui \
    Widgets/DlgLogin/FrmLoginSettings.ui \
    Widgets/FrmOptions/FrmOptions.ui \
    Widgets/FrmUservCard/FrmUservCard.ui \ 
    Widgets/WdgTableMain/WdgTableMain.ui \
    Widgets/FrmGroupChat/FrmGroupChatList.ui \
    Widgets/FrmGroupChat/FrmGroupChatFind.ui \
    Widgets/FrmGroupChat/FrmGroupChat.ui

RESOURCES += \
    Resource/Resource.qrc

win32{
SOURCES += Widgets/DlgScreenShot/DlgScreenShot.cpp
HEADERS += Widgets/DlgScreenShot/DlgScreenShot.h
}
