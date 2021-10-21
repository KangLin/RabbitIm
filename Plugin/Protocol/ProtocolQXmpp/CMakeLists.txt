#qxmpp库
#OPTION(OPTION_RABBITIM_USE_QXMPP "Use qxmpp library" ON)
PROJECT(ProtocolQXmpp)

find_package(QXmpp)
if(QXmpp_FOUND)
    if(QXmpp_VERSION VERSION_GREATER_EQUAL 1.3.0)
        message(AUTHOR_WARNING "Please use qxmpp version < v1.3.0")
        return()
    endif()
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

set(QT_COMPONENTS Network Xml)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS})
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        list(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
endif()

LIST(APPEND RABBITIM_LIBS RabbitCommon ${QT_LIBRARIES})

IF(MSVC)
    SET(RABBITIM_SYSTEM windows)
ELSEIF(MINGW)
    SET(RABBITIM_SYSTEM windows)
ELSEIF(ANDROID)
    SET(RABBITIM_SYSTEM android)
ELSE(LINUX OR UNIX)
    SET(RABBITIM_SYSTEM unix)
ENDIF()

set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/Src)
IF(ANDROID)
    LIST(APPEND INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/android/QtAndroidUtils/android/QtAndroidUtilsModule/jni)
ENDIF()

SET(SOURCE_FILES ClientXmpp.cpp
    ManageGroupChatQxmpp.cpp
    ManageUserQXmpp.cpp
    UserInfoXmpp.cpp
    GroupChatQxmpp.cpp
    PluginProtocolQXmpp.cpp
    FileTransferQXmpp.cpp
    ConvertFormat.cpp
    )

SET(HEADE_FILES
    ClientXmpp.h
    ManageGroupChatQxmpp.h
    ManageUserQXmpp.h
    UserInfoXmpp.h
    GroupChatQxmpp.h
    PluginProtocolQXmpp.h
    FileTransferQXmpp.h
    ConvertFormat.h
    )

option(RABBITIM_USE_QXMPP_CALL "Use qxmpp call" ON)
if(RABBITIM_USE_QXMPP_CALL)
    list(APPEND QXMPP_DEFINES RABBITIM_USE_QXMPP_CALL)
    list(APPEND SOURCE_FILES ManageCallXmpp.cpp CallObjectQXmpp.cpp)
    list(APPEND HEADE_FILES ManageCallXmpp.h CallObjectQXmpp.h)
endif()

file(RELATIVE_PATH PLUGIN_RELATIVE_DIR ${CMAKE_SOURCE_DIR}/Plugin ${CMAKE_CURRENT_SOURCE_DIR})
if(NOT ANDROID)
    set(PLUGIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/plugins/${PLUGIN_RELATIVE_DIR})
    set(PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/plugins/${PLUGIN_RELATIVE_DIR})
endif()

ADD_PLUGIN_TARGET(
    SOURCE_FILES ${SOURCE_FILES} ${HEADE_FILES}
    INCLUDE_DIRS ${INCLUDE_DIRS}
    PRIVATE_DEFINITIONS ${QXMPP_DEFINES} RABBITIM_SYSTEM="${RABBITIM_SYSTEM}"
    PRIVATE_LIBS RabbitIm QXmpp::QXmpp ${RABBITIM_LIBS}
    OUTPUT_DIR ${PLUGIN_OUTPUT_DIR}
    INSTALL_DIR ${PLUGIN_INSTALL_DIR}
    )
