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

RABBITIM_PLUG_NAME=CPluginProtocolQXMPP
include(../ConfigProtocol.pri)

SOURCES += \
    ClientXmpp.cpp \
    ManageCallXmpp.cpp \
    CallObjectQXmpp.cpp \
    ManageGroupChatQxmpp.cpp \
    ManageUserQXmpp.cpp \
    UserInfoXmpp.cpp \
    GroupChatQxmpp.cpp \
    PluginProtocolQXmpp.cpp \
    FileTransferQXmpp.cpp

HEADERS += \
    ClientXmpp.h \
    ManageCallXmpp.h \
    CallObjectQXmpp.h \
    ManageGroupChatQxmpp.h \
    ManageUserQXmpp.h \
    UserInfoXmpp.h \
    GroupChatQxmpp.h \
    PluginProtocolQXmpp.h \
    FileTransferQXmpp.h
