SOURCES += main.cpp\
    MainWindow.cpp \
    Tool.cpp \
    Client/ClientXmpp.cpp \
    Client/Client.cpp \
    UserInfo/UserInfo.cpp \
    UserInfo/UserInfoXmpp.cpp \
    UserInfo/COperateRoster.cpp \
    Global/Global.cpp \
    Widgets/FrmLogin/FrmLogin.cpp \
    Widgets/FrmUserList/FrmUserList.cpp \
    Widgets/FrmUserList/GroupItem.cpp \
    Widgets/FrmRegister/FrmRegister.cpp \
    Widgets/FrmAbout/FrmAbout.cpp \
    Widgets/FrmCustom/CustomTreeView.cpp \
    Widgets/FrmVideo/RecordAudio.cpp \
    Widgets/FrmLogin/FrmLoginSettings.cpp \
    Widgets/FrmUservCard/FrmUservCard.cpp \
    Widgets/FrmCustom/Label.cpp \ 
    Widgets/FrmMain/FrmMain.cpp \ 
    Manager/Manager.cpp \
    Manager/ManagerXmpp.cpp \
    Manager/ManageMessageDialog.cpp \
    Widgets/FrmMessage/FrmMessage.cpp \
    Widgets/FrmCustom/MenuMessageSend.cpp \ 
    Manager/ManageMessageDialogBigScreen.cpp \
    Widgets/FrmMessage/FrmContainer.cpp \
    Message/Message.cpp \
    Message/ChatActions/ChatAction.cpp \
    Message/ChatActions/MessageAction.cpp \
    UserInfo/User.cpp \
    Manager/ManageUser.cpp \
    Manager/ManageUserQXmpp.cpp \
    Message/SmileyPack.cpp \
    Message/EmoticonsWidget.cpp \
    Message/style.cpp \
    Widgets/FrmRecentMessage/FrmRecentMsgList.cpp \
    Widgets/FrmRecentMessage/ManageRecentMessage.cpp \
    Widgets/DlgOptions/DlgOptions.cpp \
    Widgets/DlgAddRoster/DlgAddRoster.cpp \
    FileTransfer/ManageFileTransfer.cpp \
    FileTransfer/FileTransfer.cpp \
    FileTransfer/FileTransferQXmpp.cpp \  
    Message/ChatActions/FileTransferAction.cpp \
    Widgets/FrmVideo/DataVideoBuffer.cpp \
    Widgets/FrmVideo/FrameProcess.cpp \
    Widgets/FrmVideo/CaptureVideoFrame.cpp \
    Widgets/FrmVideo/FrmPlayer.cpp \ 
    Widgets/FrmVideo/Camera.cpp \
    Call/CallObject.cpp \
    Call/CallVideo.cpp \
    Call/CallVideoQXmpp.cpp \
    Call/ManageCall.cpp \
    Call/CallAction.cpp
    #Widgets/FrmVideo/FrmVideo.cpp \ 
    #Widgets/FrmGroupChat/FrmGroupChatList.cpp \
    #Widgets/FrmGroupChat/FrmGroupChatFind.cpp \
    #Widgets/FrmGroupChat/Conference.cpp \
    #Widgets/FrmGroupChat/FrmGroupChat.cpp \
    #Widgets/FrmGroupChat/FrmCreateGroupChatRoom.cpp \
    #Widgets/DlgScreenShot/DlgScreenShot.cpp \

HEADERS  += MainWindow.h \
    Global/Global.h \
    UserInfo/UserInfo.h \
    Client/Client.h \
    UserInfo/UserInfoXmpp.h \
    Client/ClientXmpp.h \
    UserInfo/COperateRoster.h \
    Widgets/FrmMain/FrmMain.h \
    Tool.h \
    Widgets/FrmLogin/FrmLogin.h \
    Widgets/FrmUserList/FrmUserList.h \
    Widgets/FrmRegister/FrmRegister.h \
    Widgets/FrmAbout/FrmAbout.h \
    Widgets/FrmCustom/CustomTreeView.h \
    Widgets/FrmVideo/RecordAudio.h \
    Widgets/FrmAbout/Version.h \
    Widgets/FrmLogin/FrmLoginSettings.h \
    Widgets/FrmUservCard/FrmUservCard.h \ 
    Widgets/FrmCustom/Label.h \ 
    Manager/Manager.h \
    Manager/ManagerXmpp.h \
    Manager/ManageMessageDialog.h \
    Widgets/FrmCustom/MenuMessageSend.h \
    Widgets/FrmMessage/FrmMessage.h \
    Manager/ManageMessageDialogBigScreen.h \
    Widgets/FrmMessage/FrmContainer.h \
    Message/Message.h \
    Message/SmileyPack.h \
    Message/ChatActions/ChatAction.h \
    Message/ChatActions/MessageAction.h \
    UserInfo/User.h \
    Manager/ManageUser.h \
    Manager/ManageUserQXmpp.h \
    Message/EmoticonsWidget.h \
    Message/style.h \
    Widgets/FrmRecentMessage/FrmRecentMsgList.h \
    Widgets/FrmRecentMessage/ManageRecentMessage.h \
    Widgets/DlgOptions/DlgOptions.h \
    Widgets/DlgAddRoster/DlgAddRoster.h \
    FileTransfer/ManageFileTransfer.h \
    FileTransfer/FileTransfer.h \
    FileTransfer/FileTransferQXmpp.h \
    Message/ChatActions/FileTransferAction.h \
    Widgets/FrmVideo/CaptureVideoFrame.h \
    Widgets/FrmVideo/DataVideoBuffer.h \
    Widgets/FrmVideo/FrameProcess.h \
    Widgets/FrmVideo/FrmPlayer.h \    
    Widgets/FrmVideo/Camera.h \
    Call/CallObject.h \
    Call/CallVideo.h \
    Call/CallVideoQXmpp.h \
    Call/ManageCall.h \
    Call/CallAction.h
    #Widgets/FrmVideo/FrmVideo.h \ 
    #Widgets/FrmUserList/GroupItem.h \
    #Widgets/FrmGroupChat/FrmGroupChatList.h \
    #Widgets/FrmGroupChat/FrmGroupChatFind.h \
    #Widgets/FrmGroupChat/Conference.h \
    #Widgets/FrmGroupChat/FrmGroupChat.h \
    #Widgets/FrmGroupChat/FrmCreateGroupChatRoom.h \
    #Widgets/DlgScreenShot/DlgScreenShot.h \

FORMS    += MainWindow.ui \
    Widgets/FrmLogin/FrmLogin.ui \
    Widgets/FrmUserList/FrmUserList.ui \
    Widgets/FrmRegister/FrmRegister.ui \
    Widgets/FrmAbout/FrmAbout.ui \
    Widgets/FrmMain/FrmMain.ui \
    Widgets/FrmLogin/FrmLoginSettings.ui \
    Widgets/DlgOptions/DlgOptions.ui \
    Widgets/FrmUservCard/FrmUservCard.ui \
    Widgets/FrmMessage/FrmMessage.ui \
    Widgets/FrmMessage/FrmContainer.ui \
    Widgets/FrmRecentMessage/FrmRecentMsgList.ui \
    Widgets/DlgAddRoster/DlgAddRoster.ui #\
    #Widgets/FrmVideo/FrmVideo.ui 
    #Widgets/FrmGroupChat/FrmGroupChatList.ui \
    #Widgets/FrmGroupChat/FrmGroupChatFind.ui \
    #Widgets/FrmGroupChat/FrmGroupChat.ui \
    #Widgets/FrmGroupChat/FrmCreateGroupChatRoom.ui \


RESOURCES += \
    Resource/Resource.qrc

!isEmpty(RABBITIM_USER_OPENCV) {
    SOURCES += Widgets/FrmVideo/CameraOpencv.cpp 
    HEADERS += Widgets/FrmVideo/CameraOpencv.h 
}
