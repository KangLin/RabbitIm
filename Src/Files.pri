SOURCES += \
    Emoji/Emoji.cpp \
    MainWindow.cpp \
    common/Tool.cpp \
    common/QRCode.cpp \
    Client/Client.cpp \
    UserInfo/UserInfo.cpp \
    UserInfo/COperateRoster.cpp \
    Global/Global.cpp \
    Widgets/FrmLogin/FrmLogin.cpp \
    Widgets/FrmUserList/FrmUserList.cpp \
    Widgets/FrmUserList/GroupItem.cpp \
    Widgets/FrmCustom/CustomTreeView.cpp \
    Widgets/FrmCustom/Label.cpp \ 
    Widgets/FrmMain/FrmMain.cpp \ 
    Manage/Manager.cpp \
    Manage/ManageMessageDialog.cpp \
    Widgets/FrmMessage/FrmMessage.cpp \
    Widgets/FrmCustom/MenuMessageSend.cpp \ 
    Manage/ManageMessageDialogBigScreen.cpp \
    Widgets/FrmMessage/FrmContainer.cpp \
    Message/ChatActions/ChatAction.cpp \
    Message/ChatActions/MessageAction.cpp \
    UserInfo/User.cpp \
    Manage/ManageUser.cpp \
    Manage/Manage.cpp \
    Emoji/EmoticonsWidget.cpp \
    Emoji/CustomTextDocument.cpp \
    Widgets/FrmRecentMessage/FrmRecentMsgList.cpp \
    Widgets/FrmRecentMessage/ManageRecentMessage.cpp \
    Widgets/DlgAddRoster/DlgAddRoster.cpp \
    FileTransfer/ManageFileTransfer.cpp \
    FileTransfer/FileTransfer.cpp \
    FileTransfer/FileTransferAction.cpp \
    Widgets/FrmVideo/RecordAudio.cpp \
    Widgets/FrmVideo/CameraQtCaptureVideoFrame.cpp \
    Widgets/FrmVideo/ImageTool.cpp \
    Widgets/FrmVideo/FrmDisplay.cpp \
    Widgets/FrmVideo/FrmVideo.cpp \
    Call/CallObject.cpp \
    Call/ManageCall.cpp \
    Call/CallAction.cpp \
    Widgets/DlgRegister/DlgRegister.cpp \
    Widgets/FrmGroupChat/FrmGroupChatList.cpp \ 
    Widgets/FrmLogin/DlgLoginSettings.cpp \
    Widgets/FrmGroupChat/DlgCreateGroupChatRoom.cpp \
    Widgets/FrmGroupChat/ManageGroupChat.cpp \
    Widgets/FrmGroupChat/GroupChat.cpp \
    Widgets/FrmGroupChat/DlgJoinGroupChat.cpp \
    Widgets/FrmGroupChat/FrmGroupChat.cpp \
    Widgets/FrmGroupChat/ChatActionGroupChat.cpp \
    Widgets/FrmGroupChat/DlgGroupChatInfo.cpp \
    Widgets/FrmGroupChat/DlgInviter.cpp \
    Widgets/FrmCustom/ItemModelCustom.cpp \
    Widgets/DlgUservCard/DlgUservCard.cpp \ 
    Widgets/DlgOptions/DlgOptions.cpp \  
    Message/ManageMessage.cpp \
    Global/Log.cpp \
    Global/GlobalDir.cpp \
    Widgets/FrmApp/FrmApp.cpp \
    Widgets/FrmApp/FrmAppList.cpp \
    Widgets/FrmApp/FrmFavorites.cpp \
    Widgets/MainAnimation.cpp \
    ../Plugin/PluginApp.cpp \
    ../Plugin/PluginProtocol.cpp \
    ../Plugin/ManagePluginApp.cpp \
    ../Plugin/PluginConverFormat.cpp \
    ../Plugin/ManagePluginProtocol.cpp

HEADERS += \
    $$PWD/Emoji/Emoji.h \
    MainWindow.h \
    Global/Global.h \
    UserInfo/UserInfo.h \
    Client/Client.h \
    UserInfo/COperateRoster.h \
    Widgets/FrmMain/FrmMain.h \
    common/Tool.h \
    common/QRCode.h \
    Widgets/FrmLogin/FrmLogin.h \
    Widgets/FrmUserList/FrmUserList.h \
    Widgets/FrmCustom/CustomTreeView.h \
    Widgets/FrmCustom/Label.h \ 
    Manage/Manager.h \
    Manage/Manage.h \
    Manage/ManageUser.h \
    Manage/ManageMessageDialog.h \
    Widgets/FrmCustom/MenuMessageSend.h \
    Widgets/FrmMessage/FrmMessage.h \
    Manage/ManageMessageDialogBigScreen.h \
    Widgets/FrmMessage/FrmContainer.h \
    Message/ChatActions/ChatAction.h \
    Message/ChatActions/MessageAction.h \
    Emoji/EmoticonsWidget.h \
    Emoji/CustomTextDocument.h \
    UserInfo/User.h \
    Widgets/FrmRecentMessage/FrmRecentMsgList.h \
    Widgets/FrmRecentMessage/ManageRecentMessage.h \
    Widgets/DlgAddRoster/DlgAddRoster.h \
    FileTransfer/ManageFileTransfer.h \
    FileTransfer/FileTransfer.h \
    FileTransfer/FileTransferAction.h \  
    Call/CallObject.h \
    Call/ManageCall.h \
    Call/CallAction.h \
    Widgets/FrmVideo/RecordAudio.h \
    Widgets/FrmVideo/CameraQtCaptureVideoFrame.h \
    Widgets/FrmVideo/ImageTool.h \
    Widgets/FrmVideo/FrmDisplay.h \
    Widgets/FrmVideo/FrmVideo.h \ 
    Widgets/DlgRegister/DlgRegister.h \
    Widgets/FrmGroupChat/FrmGroupChatList.h \ 
    Widgets/FrmLogin/DlgLoginSettings.h \  
    Widgets/FrmGroupChat/DlgCreateGroupChatRoom.h \
    Widgets/FrmGroupChat/ManageGroupChat.h \
    Widgets/FrmGroupChat/GroupChat.h \
    Widgets/FrmGroupChat/DlgJoinGroupChat.h \
    Widgets/FrmGroupChat/FrmGroupChat.h \ 
    Widgets/FrmGroupChat/ChatActionGroupChat.h \
    Widgets/FrmGroupChat/DlgGroupChatInfo.h \
    Widgets/FrmGroupChat/DlgInviter.h \
    Widgets/FrmCustom/ItemModelCustom.h \
    Widgets/DlgUservCard/DlgUservCard.h \ 
    Widgets/DlgOptions/DlgOptions.h \ 
    Message/ManageMessage.h \
    Global/Log.h \
    Global/GlobalDir.h \
    Widgets/FrmApp/FrmApp.h \
    Widgets/FrmApp/FrmAppList.h \
    Widgets/FrmApp/FrmFavorites.h \
    Widgets/MainAnimation.h \
    ../Plugin/PluginApp.h \
    ../Plugin/PluginProtocol.h \
    ../Plugin/ManagePluginApp.h \
    ../Plugin/PluginConverFormat.h \
    ../Plugin/ManagePluginProtocol.h 

FORMS += MainWindow.ui \
    Widgets/FrmLogin/FrmLogin.ui \
    Widgets/FrmUserList/FrmUserList.ui \
    Widgets/DlgUservCard/DlgUservCard.ui \
    Widgets/FrmMessage/FrmMessage.ui \
    Widgets/FrmMessage/FrmContainer.ui \
    Widgets/FrmRecentMessage/FrmRecentMsgList.ui \
    Widgets/DlgAddRoster/DlgAddRoster.ui \
    Widgets/FrmVideo/FrmVideo.ui \ 
    Widgets/DlgRegister/DlgRegister.ui \
    Widgets/FrmGroupChat/FrmGroupChatList.ui \ 
    Widgets/FrmLogin/DlgLoginSettings.ui \ 
    Widgets/FrmGroupChat/DlgCreateGroupChatRoom.ui \
    Widgets/FrmGroupChat/FrmGroupChat.ui \
    Widgets/FrmGroupChat/DlgJoinGroupChat.ui \
    Widgets/FrmGroupChat/DlgGroupChatInfo.ui \
    Widgets/FrmGroupChat/DlgInviter.ui \
    Widgets/DlgOptions/DlgOptions.ui \
    Widgets/FrmMain/FrmMain.ui \
    Widgets/FrmApp/FrmApp.ui \
    Widgets/FrmApp/FrmAppList.ui \
    Widgets/FrmApp/FrmFavorites.ui 

RESOURCES += \
    ../Resource/Resource.qrc \
    ../Resource/sink/dark/style.qrc \

RESOURCES += \
    Emoji/Emojis/emojione.qrc \
    Emoji/Emojis/smileys.qrc

android{
    RESOURCES += \
        ../Resource/Android.qrc
       
    FORMS +=
}else{
    FORMS += 
}

equals(RABBITIM_USE_OPENSSL, 1){
    SOURCES += Global/Encrypt.cpp
    HEADERS += Global/Encrypt.h
}
