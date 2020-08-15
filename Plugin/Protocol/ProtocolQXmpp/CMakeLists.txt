#qxmpp库
#OPTION(OPTION_RABBITIM_USE_QXMPP "Use qxmpp library" ON)
#if(OPTION_RABBITIM_USE_QXMPP)
#    find_package(QXmpp)
#    if(QXmpp_FOUND)
#        ADD_DEFINITIONS("-DRABBITIM_USE_QXMPP")

#        IF(BUILD_SHARED_LIBS)
#            INSTALL(FILES $<TARGET_FILE:QXmpp::QXmpp>
#                DESTINATION "libs/${ANDROID_ABI}"
#                    COMPONENT Runtime)
#        ELSE()
#            #连接静态QXMPP库时，必须加上-DQXMPP_STATIC。
#            #生成静态QXMPP库时，qmake 需要加上 QXMPP_LIBRARY_TYPE=staticlib 参数
#            ADD_DEFINITIONS("-DQXMPP_STATIC")
#        ENDIF()
#    endif()
#ENDIF(OPTION_RABBITIM_USE_QXMPP)
#message("Use qxmpp library:${QXmpp_FOUND}")

PROJECT(ProtocolQXmpp)

set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/Src)
IF(ANDROID)
    LIST(APPEND INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/android/QtAndroidUtils/android/QtAndroidUtilsModule/jni)
ENDIF()

ADD_PLUGIN_TARGET(
    PLUGIN_TYPE Protocol
    SOURCE_FILES
        ClientXmpp.cpp
        ManageCallXmpp.cpp
        CallObjectQXmpp.cpp
        ManageGroupChatQxmpp.cpp
        ManageUserQXmpp.cpp
        UserInfoXmpp.cpp
        GroupChatQxmpp.cpp
        PluginProtocolQXmpp.cpp
        FileTransferQXmpp.cpp
    INCLUDE_DIRS
        ${INCLUDE_DIRS}
    )

