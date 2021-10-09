SET(INSTALL_HEADER_FILES
    common/QRCode.h
    common/Tool.h
    Client/Client.h
    UserInfo/UserInfo.h
    UserInfo/COperateRoster.h
    UserInfo/User.h
    Global/GlobalDir.h
    Global/Global.h
    )

SET(RABBITIM_HEADER_FILES ${INSTALL_HEADER_FILES}
    MainWindow.h
    Widgets/MainAnimation.h
    Widgets/FrmLogin/FrmLogin.h
    Widgets/FrmUserList/FrmUserList.h
    Widgets/FrmUserList/GroupItem.h
    Emoji/EmoticonsWidget.h
    Emoji/Emoji.h
    Emoji/CustomTextDocument.h

    Widgets/FrmVideo/RecordAudio.h
    Widgets/FrmVideo/CameraQtCaptureVideoFrame.h
    Widgets/FrmVideo/ImageTool.h
    Widgets/FrmVideo/FrmDisplay.h
    Widgets/FrmVideo/FrmVideo.h
    
    Manage/Manager.h 
    Manage/ManageMessageDialog.h 
    Widgets/FrmMessage/FrmMessage.h 
    Widgets/FrmCustom/MenuMessageSend.h  
    Manage/ManageMessageDialogBigScreen.h 
    
    Call/CallObject.h
    Call/ManageCall.h 
    Call/CallAction.h  
    
    ${CMAKE_SOURCE_DIR}/Plugin/PluginApp.h
    ${CMAKE_SOURCE_DIR}/Plugin/PluginProtocol.h
    ${CMAKE_SOURCE_DIR}/Plugin/PluginConverFormat.h
    ${CMAKE_SOURCE_DIR}/Plugin/ManagePluginApp.h
    ${CMAKE_SOURCE_DIR}/Plugin/ManagePluginProtocol.h
    )

SET(RABBITIM_SOURCES
    MainWindow.cpp 
    common/Tool.cpp 
    common/QRCode.cpp 
    Client/Client.cpp 
    UserInfo/UserInfo.cpp 
    UserInfo/COperateRoster.cpp 
    Global/Global.cpp 
    Widgets/FrmLogin/FrmLogin.cpp 
    Widgets/FrmUserList/FrmUserList.cpp 
    Widgets/FrmUserList/GroupItem.cpp 
    Widgets/FrmCustom/CustomTreeView.cpp
    Widgets/FrmCustom/Label.cpp  
    Widgets/FrmMain/FrmMain.cpp  
    Manage/Manager.cpp 
    Manage/ManageMessageDialog.cpp 
    Widgets/FrmMessage/FrmMessage.cpp 
    Widgets/FrmCustom/MenuMessageSend.cpp  
    Manage/ManageMessageDialogBigScreen.cpp 
    Widgets/FrmMessage/FrmContainer.cpp 
    Message/ChatActions/ChatAction.cpp 
    Message/ChatActions/MessageAction.cpp 
    UserInfo/User.cpp 
    Manage/ManageUser.cpp 
    Manage/Manage.cpp 
    Emoji/Emoji.cpp
    Emoji/EmoticonsWidget.cpp
    Emoji/CustomTextDocument.cpp
    Widgets/FrmRecentMessage/FrmRecentMsgList.cpp 
    Widgets/FrmRecentMessage/ManageRecentMessage.cpp 
    Widgets/DlgAddRoster/DlgAddRoster.cpp 
    FileTransfer/ManageFileTransfer.cpp 
    FileTransfer/FileTransfer.cpp 
    FileTransfer/FileTransferAction.cpp 
    Widgets/FrmVideo/RecordAudio.cpp
    Widgets/FrmVideo/CameraQtCaptureVideoFrame.cpp
    Widgets/FrmVideo/ImageTool.cpp
    Widgets/FrmVideo/FrmDisplay.cpp
    Widgets/FrmVideo/FrmVideo.cpp
    Call/CallObject.cpp
    Call/ManageCall.cpp 
    Call/CallAction.cpp  
    Widgets/DlgRegister/DlgRegister.cpp 
    Widgets/FrmGroupChat/FrmGroupChatList.cpp  
    Widgets/FrmLogin/DlgLoginSettings.cpp 
    Widgets/FrmGroupChat/DlgCreateGroupChatRoom.cpp 
    Widgets/FrmGroupChat/ManageGroupChat.cpp 
    Widgets/FrmGroupChat/GroupChat.cpp 
    Widgets/FrmGroupChat/DlgJoinGroupChat.cpp 
    Widgets/FrmGroupChat/FrmGroupChat.cpp 
    Widgets/FrmGroupChat/ChatActionGroupChat.cpp 
    Widgets/FrmGroupChat/DlgGroupChatInfo.cpp 
    Widgets/FrmGroupChat/DlgInviter.cpp 
    Widgets/FrmCustom/ItemModelCustom.cpp 
    Widgets/DlgUservCard/DlgUservCard.cpp  
    Widgets/DlgOptions/DlgOptions.cpp   
    Message/ManageMessage.cpp
    Global/GlobalDir.cpp 
    Widgets/FrmApp/FrmApp.cpp 
    Widgets/FrmApp/FrmAppList.cpp 
    Widgets/FrmApp/FrmFavorites.cpp 
    ${CMAKE_SOURCE_DIR}/Plugin/PluginApp.cpp 
    ${CMAKE_SOURCE_DIR}/Plugin/PluginProtocol.cpp 
    ${CMAKE_SOURCE_DIR}/Plugin/PluginConverFormat.cpp
    ${CMAKE_SOURCE_DIR}/Plugin/ManagePluginApp.cpp 
    ${CMAKE_SOURCE_DIR}/Plugin/ManagePluginProtocol.cpp
    Widgets/MainAnimation.cpp
)

SET(RABBITIM_UIS
    MainWindow.ui 
    Widgets/FrmLogin/FrmLogin.ui 
    Widgets/FrmUserList/FrmUserList.ui 
    Widgets/DlgUservCard/DlgUservCard.ui 
    Widgets/FrmMessage/FrmMessage.ui 
    Widgets/FrmMessage/FrmContainer.ui 
    Widgets/FrmRecentMessage/FrmRecentMsgList.ui 
    Widgets/DlgAddRoster/DlgAddRoster.ui 
    Widgets/FrmVideo/FrmVideo.ui  
    Widgets/DlgRegister/DlgRegister.ui 
    Widgets/FrmGroupChat/FrmGroupChatList.ui  
    Widgets/FrmLogin/DlgLoginSettings.ui  
    Widgets/FrmGroupChat/DlgCreateGroupChatRoom.ui 
    Widgets/FrmGroupChat/FrmGroupChat.ui 
    Widgets/FrmGroupChat/DlgJoinGroupChat.ui 
    Widgets/FrmGroupChat/DlgGroupChatInfo.ui 
    Widgets/FrmGroupChat/DlgInviter.ui 
    Widgets/DlgOptions/DlgOptions.ui 
    Widgets/FrmMain/FrmMain.ui 
    Widgets/FrmApp/FrmApp.ui 
    Widgets/FrmApp/FrmAppList.ui 
    Widgets/FrmApp/FrmFavorites.ui 
)

find_package(OpenSSL)
IF(OpenSSL_FOUND)
    list(APPEND RABBITIM_LIBS OpenSSL::SSL OpenSSL::Crypto)
    list(APPEND RABBITIM_DEFINES RABBITIM_USE_OPENSSL)
    # QtCreator supports the following variables for Android, which are identical to qmake Android variables.
    # Check https://doc.qt.io/qt/deployment-android.html for more information.
    # They need to be set before the find_package( ...) calls below.
    if(ANDROID)
        #set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
        if (ANDROID_ABI STREQUAL "armeabi-v7a")
            set(ANDROID_EXTRA_LIBS
                $<TARGET_FILE:OpenSSL::SSL>
                $<TARGET_FILE:OpenSSL::Crypto>)
        endif()
    endif()
    
    if(WIN32)
        INSTALL_TARGETS(TARGETS OpenSSL::SSL OpenSSL::Crypto)
    endif()
    
    LIST(APPEND RABBITIM_SOURCES Global/Encrypt.cpp)
    LIST(APPEND RABBITIM_HEADER_FILES Global/Encrypt.h)
ENDIF()
