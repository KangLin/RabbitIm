PROJECT(ProtocolQXmpp)

SET(RABBITIM_PLUG_NAME CPluginProtocolQXMPP)

SET(PLUGIN_SOURCES
        ClientXmpp.cpp
        ManageCallXmpp.cpp
        CallObjectQXmpp.cpp
        ManageGroupChatQxmpp.cpp
        ManageUserQXmpp.cpp
        UserInfoXmpp.cpp
        GroupChatQxmpp.cpp
        PluginProtocolQXmpp.cpp
        FileTransferQXmpp.cpp
    )

include(${CMAKE_SOURCE_DIR}/cmake/PluginCommon.cmake)
