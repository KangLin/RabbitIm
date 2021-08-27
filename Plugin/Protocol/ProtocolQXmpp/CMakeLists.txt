#qxmpp库
#OPTION(OPTION_RABBITIM_USE_QXMPP "Use qxmpp library" ON)
PROJECT(ProtocolQXmpp)

find_package(QXmpp)
if(QXmpp_FOUND)
    list(APPEND QXMPP_DEFINES RABBITIM_USE_QXMPP)
    
    IF(BUILD_SHARED_LIBS)
        INSTALL_TARGETS(TARGETS QXmpp::QXmpp)
    ELSE()
        #连接静态QXMPP库时，必须加上-DQXMPP_STATIC。
        #生成静态QXMPP库时，qmake 需要加上 QXMPP_LIBRARY_TYPE=staticlib 参数
        list(APPEND QXMPP_DEFINES "-DQXMPP_STATIC")
    ENDIF()
else()
    return()
endif()

message("Use qxmpp library:${QXmpp_FOUND}")

set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/Src)
IF(ANDROID)
    LIST(APPEND INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/android/QtAndroidUtils/android/QtAndroidUtilsModule/jni)
ENDIF()

SET(SOURCES ClientXmpp.cpp
    ManageGroupChatQxmpp.cpp
    ManageUserQXmpp.cpp
    UserInfoXmpp.cpp
    GroupChatQxmpp.cpp
    PluginProtocolQXmpp.cpp
    FileTransferQXmpp.cpp)

option(RABBITIM_USE_QXMPP_CALL "Use qxmpp call" ON)
if(RABBITIM_USE_QXMPP_CALL)
    list(APPEND QXMPP_DEFINES RABBITIM_USE_QXMPP_CALL)
    list(APPEND SOURCES ManageCallXmpp.cpp CallObjectQXmpp.cpp)
endif()

ADD_PLUGIN_TARGET(
    PLUGIN_TYPE Protocol
    SOURCE_FILES ${SOURCES}
    INCLUDE_DIRS ${INCLUDE_DIRS}
    PRIVATE_DEFINITIONS ${QXMPP_DEFINES}
    )

