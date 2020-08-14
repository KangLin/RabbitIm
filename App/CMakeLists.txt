cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0083)
    cmake_policy(SET CMP0083 NEW)
endif()

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()

project(RabbitImApp)

set(SOURCES_FILES
    main.cpp
    )

SET(PRIVATE_DEFINITIONS
    BUILD_VERSION="${BUILD_VERSION}"
    BUILD_ARCH="${BUILD_ARCH}"
    BUILD_PLATFORM="${BUILD_PLATFORM}"
    )
if(MSVC)
    list(APPEND PRIVATE_DEFINITIONS "$<$<C_COMPILER_ID:MSVC>:/utf-8>"
         "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

LIST(APPEND RCC_FILES ${CMAKE_SOURCE_DIR}/Resource/Resource.qrc)
if(WIN32)
    LIST(APPEND RCC_FILES AppIcon.rc)
endif()
#翻译
GENERATED_QT_TRANSLATIONS(SOURCES ${SOURCES_FILES}
    OUT_QRC TRANSLATIONS_QRC_FILES)
string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
if(BUILD_TYPE STREQUAL "debug")
    LIST(APPEND PRIVATE_DEFINITIONS DEBUG)
    LIST(APPEND RCC_FILES
        ${TRANSLATIONS_QRC_FILES}
        )
endif()

SET(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/Src
    ${RabbitCommon_DIR}/Src
    ${CMAKE_BINARY_DIR}
    )
if(ANDROID)
    list(APPEND INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/android/QtAndroidUtils/android/QtAndroidUtilsModule/jni)
endif()

ADD_TARGET(NAME ${PROJECT_NAME}
    ISEXE
    SOURCE_FILES ${SOURCES_FILES} ${RCC_FILES}
    PRIVATE_LIBS ${QT_LIBRARIES} RabbitIm
    PRIVATE_DEFINITIONS ${PRIVATE_DEFINITIONS}
    INCLUDE_DIRS ${INCLUDE_DIRS}
    VERSION ${BUILD_VERSION}
    ANDROID_SOURCES_DIR ${CMAKE_SOURCE_DIR}/android
    )

IF(WIN32)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/Install/Install.nsi
        DESTINATION ${CMAKE_BINARY_DIR}
            COMPONENT Runtime)

    INSTALL(FILES ${CMAKE_SOURCE_DIR}/Resource/png/RabbitIm.ico
        DESTINATION .
            COMPONENT Runtime)
  
    #安装第三方库
    INSTALL(DIRECTORY ${THIRD_LIBRARY_PATH}/bin/
        ${THIRD_LIBRARY_PATH}/lib/
        DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
        FILES_MATCHING PATTERN "*.dll")

    IF(NOT EXISTS ${CMAKE_BINARY_DIR}/bin/qxmpp.dll)
        FILE(COPY ${THIRD_LIBRARY_PATH}/bin/
            ${THIRD_LIBRARY_PATH}/lib/
            DESTINATION ${CMAKE_BINARY_DIR}/bin
            FILES_MATCHING PATTERN "*.dll")
    ENDIF()
ENDIF()

if(UNIX AND NOT ANDROID)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/debian/RabbitIm.desktop
        DESTINATION "share/applications"
            COMPONENT Runtime)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/Resource/png/RabbitIm.png
        DESTINATION "share/pixmaps"
            COMPONENT Runtime)
endif()
