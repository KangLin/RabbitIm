#-------------------------------------------------
#
# Project created by QtCreator 2015-12-18T13:15:27
#
#-------------------------------------------------

QT *= core

TARGET = ProtocolQXmpp
TEMPLATE = lib
DISTFILES += \
    ProtocolQXmpp.json
OTHER_FILES += CMakeLists.txt

RABBITIM_PLUG_NAME=CPluginProtocolQXMPP
include(../../pri/ConfigProtocol.pri)

SOURCES += \
    ClientXmpp.cpp \
    ManageGroupChatQxmpp.cpp \
    ManageUserQXmpp.cpp \
    UserInfoXmpp.cpp \
    GroupChatQxmpp.cpp \
    PluginProtocolQXmpp.cpp \
    FileTransferQXmpp.cpp \
    ConvertFormat.cpp

HEADERS += \
    ClientXmpp.h \
    ManageGroupChatQxmpp.h \
    ManageUserQXmpp.h \
    UserInfoXmpp.h \
    GroupChatQxmpp.h \
    PluginProtocolQXmpp.h \
    FileTransferQXmpp.h \
    ConvertFormat.h

CONFIG*=RABBITIM_USE_QXMPP_CALL

RABBITIM_USE_QXMPP_CALL {
    DEFINES *= RABBITIM_USE_QXMPP_CALL
    RABBITIM_USE_WEBRTC {
        SOURCES += QXmppWebRtcIq.cpp \
            ManageCallWebrtcXmpp.cpp \
            CallObjectQXmppWebrtc.cpp \
            QXmppCallWebrtcManager.cpp \
            WebrtcConductor.cpp \
            VideoRenderer.cpp \
            WebrtcFilter.cpp \
            WebrtcQtSocketServer.cpp

        HEADERS += \
            QXmppWebRtcIq.h \
            ManageCallWebrtcXmpp.h \
            CallObjectQXmppWebrtc.h \
            QXmppCallWebrtcManager.h \
            WebrtcConductor.h \
            MediaConstraints.h \
            VideoRenderer.h \
            WebrtcFilter.h \
            WebrtcQtSocketServer.h
    }else{
        SOURCES += ManageCallXmpp.cpp \
            CallObjectQXmpp.cpp

        HEADERS += ManageCallXmpp.h \
            CallObjectQXmpp.h
    }
}
