SOURCES += \
    $$PWD/../MainWindow.cpp \
    $$PWD/../common/Tool.cpp \
    $$PWD/../common/QRCode.cpp \
    $$PWD/../Client/Client.cpp \
    $$PWD/../UserInfo/UserInfo.cpp \
    $$PWD/../UserInfo/COperateRoster.cpp \
    $$PWD/../Global/Global.cpp \
    $$PWD/../Widgets/FrmLogin/FrmLogin.cpp \
    $$PWD/../Widgets/FrmUserList/FrmUserList.cpp \
    $$PWD/../Widgets/FrmUserList/GroupItem.cpp \
    $$PWD/../Widgets/FrmCustom/CustomTreeView.cpp \
    $$PWD/../Widgets/FrmVideo/RecordAudio.cpp \
    $$PWD/../Widgets/FrmCustom/Label.cpp \ 
    $$PWD/../Widgets/FrmMain/FrmMain.cpp \ 
    $$PWD/../Manage/Manager.cpp \
    $$PWD/../Manage/ManageMessageDialog.cpp \
    $$PWD/../Widgets/FrmMessage/FrmMessage.cpp \
    $$PWD/../Widgets/FrmCustom/MenuMessageSend.cpp \ 
    $$PWD/../Manage/ManageMessageDialogBigScreen.cpp \
    $$PWD/../Widgets/FrmMessage/FrmContainer.cpp \
    $$PWD/../Message/ChatActions/ChatAction.cpp \
    $$PWD/../Message/ChatActions/MessageAction.cpp \
    $$PWD/../UserInfo/User.cpp \
    $$PWD/../Manage/ManageUser.cpp \
    $$PWD/../Manage/Manage.cpp \
    $$PWD/../Message/SmileyPack.cpp \
    $$PWD/../Message/EmoticonsWidget.cpp \
    $$PWD/../Message/style.cpp \
    $$PWD/../Widgets/FrmRecentMessage/FrmRecentMsgList.cpp \
    $$PWD/../Widgets/FrmRecentMessage/ManageRecentMessage.cpp \
    $$PWD/../Widgets/DlgAddRoster/DlgAddRoster.cpp \
    $$PWD/../FileTransfer/ManageFileTransfer.cpp \
    $$PWD/../FileTransfer/FileTransfer.cpp \
    $$PWD/../FileTransfer/FileTransferAction.cpp \
    $$PWD/../Widgets/FrmVideo/DataVideoBuffer.cpp \
    $$PWD/../Widgets/FrmVideo/FrameProcess.cpp \
    $$PWD/../Widgets/FrmVideo/FrmPlayer.cpp \ 
    $$PWD/../Call/CallObject.cpp \
    $$PWD/../Call/ManageCall.cpp \
    $$PWD/../Call/CallAction.cpp \
    $$PWD/../Widgets/FrmVideo/FrmVideo.cpp \ 
    $$PWD/../Widgets/DlgRegister/DlgRegister.cpp \
    $$PWD/../Widgets/FrmGroupChat/FrmGroupChatList.cpp \ 
    $$PWD/../Widgets/FrmLogin/DlgLoginSettings.cpp \
    $$PWD/../Widgets/FrmGroupChat/DlgCreateGroupChatRoom.cpp \
    $$PWD/../Widgets/FrmGroupChat/ManageGroupChat.cpp \
    $$PWD/../Widgets/FrmGroupChat/GroupChat.cpp \
    $$PWD/../Widgets/FrmGroupChat/DlgJoinGroupChat.cpp \
    $$PWD/../Widgets/FrmGroupChat/FrmGroupChat.cpp \
    $$PWD/../Widgets/FrmGroupChat/ChatActionGroupChat.cpp \
    $$PWD/../Widgets/FrmGroupChat/DlgGroupChatInfo.cpp \
    $$PWD/../Widgets/FrmGroupChat/DlgInviter.cpp \
    $$PWD/../Widgets/FrmCustom/ItemModelCustom.cpp \
    $$PWD/../Widgets/DlgUservCard/DlgUservCard.cpp \ 
    $$PWD/../Widgets/DlgAbout/DlgAbout.cpp \
    $$PWD/../Widgets/DlgOptions/DlgOptions.cpp \  
    $$PWD/../Message/ManageMessage.cpp \
    $$PWD/../Media/Camera/Camera.cpp \
    $$PWD/../Media/Camera/CameraInfo.cpp \
    $$PWD/../Media/Camera/CameraFactory.cpp \
    $$PWD/../Media/Camera/VideoFrame.cpp \
    $$PWD/../Media/Camera/CameraQt.cpp \
    $$PWD/../Media/Camera/CameraFactoryQt.cpp \
    $$PWD/../Media/Camera/CameraQtCaptureVideoFrame.cpp \
    $$PWD/../Global/Log.cpp \
    $$PWD/../Global/GlobalDir.cpp \
    $$PWD/../Widgets/FrmApp/FrmApp.cpp \
    $$PWD/../Widgets/FrmApp/FrmAppList.cpp \
    $$PWD/../Widgets/FrmApp/FrmFavorites.cpp \
    $$PWD/../Plugin/PluginApp.cpp \
    $$PWD/../Plugin/PluginProtocol.cpp \
    $$PWD/../Plugin/ManagePluginApp.cpp \
    $$PWD/../Plugin/ManagePluginProtocol.cpp \
    $$PWD/../Widgets/MainAnimation.cpp

HEADERS += $$PWD/../Version.h \
    $$PWD/../MainWindow.h \
    $$PWD/../Global/Global.h \
    $$PWD/../UserInfo/UserInfo.h \
    $$PWD/../Client/Client.h \
    $$PWD/../UserInfo/COperateRoster.h \
    $$PWD/../Widgets/FrmMain/FrmMain.h \
    $$PWD/../common/Tool.h \
    $$PWD/../common/QRCode.h \
    $$PWD/../Widgets/FrmLogin/FrmLogin.h \
    $$PWD/../Widgets/FrmUserList/FrmUserList.h \
    $$PWD/../Widgets/FrmCustom/CustomTreeView.h \
    $$PWD/../Widgets/FrmVideo/RecordAudio.h \
    $$PWD/../Widgets/FrmCustom/Label.h \ 
    $$PWD/../Manage/Manager.h \
    $$PWD/../Manage/Manage.h \
    $$PWD/../Manage/ManageUser.h \
    $$PWD/../Manage/ManageMessageDialog.h \
    $$PWD/../Widgets/FrmCustom/MenuMessageSend.h \
    $$PWD/../Widgets/FrmMessage/FrmMessage.h \
    $$PWD/../Manage/ManageMessageDialogBigScreen.h \
    $$PWD/../Widgets/FrmMessage/FrmContainer.h \
    $$PWD/../Message/SmileyPack.h \
    $$PWD/../Message/ChatActions/ChatAction.h \
    $$PWD/../Message/ChatActions/MessageAction.h \
    $$PWD/../UserInfo/User.h \
    $$PWD/../Message/EmoticonsWidget.h \
    $$PWD/../Message/style.h \
    $$PWD/../Widgets/FrmRecentMessage/FrmRecentMsgList.h \
    $$PWD/../Widgets/FrmRecentMessage/ManageRecentMessage.h \
    $$PWD/../Widgets/DlgAddRoster/DlgAddRoster.h \
    $$PWD/../FileTransfer/ManageFileTransfer.h \
    $$PWD/../FileTransfer/FileTransfer.h \
    $$PWD/../FileTransfer/FileTransferAction.h \
    $$PWD/../Widgets/FrmVideo/DataVideoBuffer.h \
    $$PWD/../Widgets/FrmVideo/FrameProcess.h \
    $$PWD/../Widgets/FrmVideo/FrmPlayer.h \    
    $$PWD/../Call/CallObject.h \
    $$PWD/../Call/ManageCall.h \
    $$PWD/../Call/CallAction.h \ 
    $$PWD/../Widgets/FrmVideo/FrmVideo.h \ 
    $$PWD/../Widgets/DlgRegister/DlgRegister.h \
    $$PWD/../Widgets/FrmGroupChat/FrmGroupChatList.h \ 
    $$PWD/../Widgets/FrmLogin/DlgLoginSettings.h \  
    $$PWD/../Widgets/FrmGroupChat/DlgCreateGroupChatRoom.h \
    $$PWD/../Widgets/FrmGroupChat/ManageGroupChat.h \
    $$PWD/../Widgets/FrmGroupChat/GroupChat.h \
    $$PWD/../Widgets/FrmGroupChat/DlgJoinGroupChat.h \
    $$PWD/../Widgets/FrmGroupChat/FrmGroupChat.h \ 
    $$PWD/../Widgets/FrmGroupChat/ChatActionGroupChat.h \
    $$PWD/../Widgets/FrmGroupChat/DlgGroupChatInfo.h \
    $$PWD/../Widgets/FrmGroupChat/DlgInviter.h \
    $$PWD/../Widgets/FrmCustom/ItemModelCustom.h \
    $$PWD/../Widgets/DlgUservCard/DlgUservCard.h \ 
    $$PWD/../Widgets/DlgAbout/DlgAbout.h \
    $$PWD/../Widgets/DlgOptions/DlgOptions.h \ 
    $$PWD/../Message/ManageMessage.h \
    $$PWD/../Media/Camera/Camera.h \
    $$PWD/../Media/Camera/CameraInfo.h \
    $$PWD/../Media/Camera/VideoType.h \
    $$PWD/../Media/Camera/CameraFactory.h \
    $$PWD/../Media/Camera/VideoFrame.h \
    $$PWD/../Media/Camera/CameraQt.h \
    $$PWD/../Media/Camera/CameraFactoryQt.h \
    $$PWD/../Media/Camera/CameraQtCaptureVideoFrame.h \
    $$PWD/../Global/Log.h \
    $$PWD/../Global/GlobalDir.h \
    $$PWD/../Widgets/FrmApp/FrmApp.h \
    $$PWD/../Widgets/FrmApp/FrmAppList.h \
    $$PWD/../Widgets/FrmApp/FrmFavorites.h \
    $$PWD/../Plugin/PluginApp.h \
    $$PWD/../Plugin/PluginProtocol.h \
    $$PWD/../Plugin/ManagePluginApp.h \
    $$PWD/../Plugin/ManagePluginProtocol.h \
    $$PWD/../Widgets/MainAnimation.h

FORMS += $$PWD/../MainWindow.ui \
    $$PWD/../Widgets/FrmLogin/FrmLogin.ui \
    $$PWD/../Widgets/FrmUserList/FrmUserList.ui \
    $$PWD/../Widgets/DlgUservCard/DlgUservCard.ui \
    $$PWD/../Widgets/FrmMessage/FrmMessage.ui \
    $$PWD/../Widgets/FrmMessage/FrmContainer.ui \
    $$PWD/../Widgets/FrmRecentMessage/FrmRecentMsgList.ui \
    $$PWD/../Widgets/DlgAddRoster/DlgAddRoster.ui \
    $$PWD/../Widgets/FrmVideo/FrmVideo.ui \ 
    $$PWD/../Widgets/DlgRegister/DlgRegister.ui \
    $$PWD/../Widgets/FrmGroupChat/FrmGroupChatList.ui \ 
    $$PWD/../Widgets/FrmLogin/DlgLoginSettings.ui \ 
    $$PWD/../Widgets/FrmGroupChat/DlgCreateGroupChatRoom.ui \
    $$PWD/../Widgets/FrmGroupChat/FrmGroupChat.ui \
    $$PWD/../Widgets/FrmGroupChat/DlgJoinGroupChat.ui \
    $$PWD/../Widgets/FrmGroupChat/DlgGroupChatInfo.ui \
    $$PWD/../Widgets/FrmGroupChat/DlgInviter.ui \
    $$PWD/../Widgets/DlgAbout/DlgAbout.ui \
    $$PWD/../Widgets/DlgOptions/DlgOptions.ui \
    $$PWD/../Widgets/FrmMain/FrmMain.ui \
    $$PWD/../Widgets/FrmApp/FrmApp.ui \
    $$PWD/../Widgets/FrmApp/FrmAppList.ui \
    $$PWD/../Widgets/FrmApp/FrmFavorites.ui 

RESOURCES += \
    $$PWD/../Resource/Resource.qrc \
    $$PWD/../Resource/sink/dark/style.qrc

android{
    RESOURCES += \
        $$PWD/../Resource/translations/Translations.qrc \
        $$PWD/../Resource/Android.qrc
    FORMS +=
}else{
    FORMS += 
}

#equals(RABBITIM_USE_OPENCV, 1){
#    SOURCES += Widgets/FrmVideo/CameraOpencv.cpp 
#    HEADERS += Widgets/FrmVideo/CameraOpencv.h 
#}

equals(RABBITIM_USE_OPENSSL, 1){
    SOURCES += $$PWD/../Global/Encrypt.cpp
    HEADERS += $$PWD/../Global/Encrypt.h
}

equals(RABBITIM_USE_PJSIP, 1) {
    SOURCES += \
        $$PWD/../Media/Camera/CameraPjsip.cpp \
        $$PWD/../Media/Camera/CameraFactoryPjsip.cpp
    HEADERS += \
        $$PWD/../Media/Camera/CameraPjsip.h \
        $$PWD/../Media/Camera/CameraFactoryPjsip.h
}
