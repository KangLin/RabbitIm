# Author: Kang Lin(kl222@126.com)

project(RabbitImApp)

set(SOURCES_FILES
    main.cpp
    )

SET(PRIVATE_DEFINITIONS
    RabbitIm_VERSION="${RabbitIm_VERSION}"
    RabbitIm_REVISION="${RabbitIm_REVISION}"
    )

LIST(APPEND RCC_FILES ${CMAKE_SOURCE_DIR}/Resource/Resource.qrc)
if(WIN32)
    LIST(APPEND RCC_FILES AppIcon.rc)
endif()

SET(INCLUDE_DIRS
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Src>
    $<BUILD_INTERFACE:${RabbitCommon_DIR}/Src>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    )
if(ANDROID)
    list(APPEND INCLUDE_DIRS $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/android/QtAndroidUtils/android/QtAndroidUtilsModule/jni>)
endif()

set(APP_ID io.github.KangLin.RabbitIm)
ADD_TARGET(NAME ${PROJECT_NAME}
    ISEXE
    SOURCE_FILES ${SOURCES_FILES} ${RCC_FILES}
    PRIVATE_LIBS ${QT_LIBRARIES} RabbitIm
    PRIVATE_DEFINITIONS ${PRIVATE_DEFINITIONS}
    INCLUDE_DIRS ${INCLUDE_DIRS}
    VERSION ${RabbitIm_VERSION}
    ANDROID_SOURCES_DIR ${CMAKE_SOURCE_DIR}/android
    )

IF(WIN32)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/Package/Windows.nsi
        DESTINATION ${CMAKE_BINARY_DIR}
            COMPONENT Runtime)

    INSTALL(FILES ${CMAKE_SOURCE_DIR}/Resource/png/RabbitIm.ico
        DESTINATION .
            COMPONENT Runtime)
ENDIF()

if(UNIX AND NOT ANDROID)
    configure_file(${CMAKE_SOURCE_DIR}/Package/share/applications/${APP_ID}.desktop.in
        ${CMAKE_BINARY_DIR}/${APP_ID}.desktop)
    INSTALL(FILES ${CMAKE_BINARY_DIR}/${APP_ID}.desktop
        DESTINATION share/applications
            COMPONENT Application)
    # Flatpak: https://docs.flatpak.org/en/latest/conventions.html#application-icons
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/Resource/png/RabbitIm.png
        DESTINATION share/pixmaps
            COMPONENT Application
        RENAME ${APP_ID}.png)
    string(TIMESTAMP BUILD_DATE "%Y-%m-%d")
    configure_file(${CMAKE_SOURCE_DIR}/Package/share/metainfo/${APP_ID}.metainfo.xml.in
        ${CMAKE_BINARY_DIR}/${APP_ID}.metainfo.xml @ONLY)
    INSTALL(FILES ${CMAKE_BINARY_DIR}/${APP_ID}.metainfo.xml
        DESTINATION "${CMAKE_INSTALL_DATADIR}/metainfo"
        COMPONENT Application)
endif()

#安装相关文件
set(OTHER_FILES
    ${CMAKE_SOURCE_DIR}/License.md
    ${CMAKE_SOURCE_DIR}/Authors.md
    ${CMAKE_SOURCE_DIR}/ChangeLog.md
    )
INSTALL_FILE(SOURCES ${OTHER_FILES}
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT Application)
