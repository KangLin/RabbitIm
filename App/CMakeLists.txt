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

LIST(APPEND RCC_FILES ${CMAKE_SOURCE_DIR}/Resource/Resource.qrc)
if(WIN32)
    LIST(APPEND RCC_FILES AppIcon.rc)
endif()
#翻译
include(${RabbitCommon_DIR}/cmake/Translations.cmake)
GENERATED_QT_TRANSLATIONS(SOURCES ${SOURCES_FILES}
    OUT_QRC TRANSLATIONS_QRC_FILES)
string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
if(BUILD_TYPE STREQUAL "debug")
    LIST(APPEND RCC_FILES
        ${TRANSLATIONS_QRC_FILES}
        )
endif()

add_definitions(-DBUILD_VERSION="${BUILD_VERSION}")
add_definitions(-DBUILD_ARCH="${BUILD_ARCH}")
add_definitions(-DBUILD_PLATFORM="${BUILD_PLATFORM}")
if(ANDROID)
    add_library(${PROJECT_NAME} ${SOURCES_FILES} ${HEADER_FILES} ${SOURCE_UI_FILES} ${RCC_FILES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES_FILES} ${HEADER_FILES} ${SOURCE_UI_FILES} ${RCC_FILES})
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBRARIES} RabbitIm)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    VERSION ${BUILD_VERSION}
    )
if(BUILD_VERSION)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DBUILD_VERSION="${BUILD_VERSION}")
endif()
if(UNIX AND NOT ANDROID)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_NAME ${PROJECT_NAME})
endif()

if(MINGW)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-mwindows")
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

    if(Qt5_VERSION VERSION_LESS "5.7.0")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS
            "/SUBSYSTEM:WINDOWS\",5.01\" /ENTRY:mainCRTStartup")
    endif()
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/Src
    ${RabbitCommon_DIR}/Src
    ${CMAKE_BINARY_DIR})

# Install target
if(ANDROID)
    INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "libs/${ANDROID_ABI}"
            COMPONENT Runtime
        LIBRARY DESTINATION "libs/${ANDROID_ABI}"
            COMPONENT Runtime
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
elseif(WIN32)
    INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime
        LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
else()
    INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            COMPONENT Runtime
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )        
endif()

IF(WIN32)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/Resource/png/RabbitIm.ico
        ${CMAKE_SOURCE_DIR}/Install/Install.nsi
        DESTINATION ${CMAKE_BINARY_DIR}
            COMPONENT Runtime)

    #安装第三方库
    INSTALL(DIRECTORY ${THIRD_LIBRARY_PATH}/bin/
        ${THIRD_LIBRARY_PATH}/lib/
        DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
        FILES_MATCHING PATTERN "*.dll")
ENDIF()

if(UNIX AND NOT ANDROID)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/debian/RabbitIm.desktop
        DESTINATION "share/applications"
            COMPONENT Runtime)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/Resource/png/RabbitIm.png
        DESTINATION "share/pixmaps"
            COMPONENT Runtime)
endif()

IF(ANDROID)
    Set(JSON_FILE ${CMAKE_CURRENT_BINARY_DIR}/android_deployment_settings.json)
    GENERATED_DEPLOYMENT_SETTINGS(NAME ${JSON_FILE}
        APPLACTION "${CMAKE_BINARY_DIR}/bin/lib${PROJECT_NAME}.so")

    add_custom_target(APK #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
            COMMAND "${QT_INSTALL_DIR}/bin/androiddeployqt"
                --output ${CMAKE_INSTALL_PREFIX}
                --input ${JSON_FILE}
                --verbose
                --gradle
                --android-platform ${ANDROID_PLATFORM}
        )
ENDIF(ANDROID)

IF( ("Release" STREQUAL CMAKE_BUILD_TYPE)
    OR ("MinSizeRel" STREQUAL CMAKE_BUILD_TYPE)
    AND WIN32 AND BUILD_SHARED_LIBS )
    IF( ("Release" STREQUAL CMAKE_BUILD_TYPE)
        OR ("MinSizeRel" STREQUAL CMAKE_BUILD_TYPE)
        AND MINGW )
        #windeployqt 分发时，是根据是否 strip 来判断是否是 DEBUG 版本,而用mingw编译时,qt没有自动 strip
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND strip "$<TARGET_FILE:${PROJECT_NAME}>"
            )
    ENDIF()

    #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中  
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${QT_INSTALL_DIR}/bin/windeployqt"
        --compiler-runtime
        --verbose 7
        "$<TARGET_FILE:${PROJECT_NAME}>"
        )
    INSTALL(DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
        DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Runtime)
ENDIF()
