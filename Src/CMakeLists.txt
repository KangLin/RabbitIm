cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()

project(RabbitIm)

include(Files.cmake)

#资源
LIST(APPEND RABBITIM_RCC_FILES ${CMAKE_SOURCE_DIR}/Resource/Resource.qrc)
#翻译资源
include(${RabbitCommon_DIR}/cmake/Translations.cmake)
GENERATED_QT_TRANSLATIONS(SOURCES ${RABBITIM_SOURCES} ${RABBITIM_UIS}
    OUT_QRC TRANSLATIONS_QRC_FILES)
string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
if(BUILD_TYPE STREQUAL "debug")
    LIST(APPEND RABBITIM_DEFINES DEBUG)
    LIST(APPEND RABBITIM_RCC_FILES
        ${TRANSLATIONS_QRC_FILES}
        )
endif()

add_library(${PROJECT_NAME} ${RABBITIM_SOURCES} ${HEADER_FILES} ${RABBITIM_UIS} ${RABBITIM_RCC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    PUBLIC_HEADER "${INSTALL_HEADER_FILES}" # Install head files
    VERSION ${BUILD_VERSION})

target_compile_definitions(${PROJECT_NAME} PUBLIC RABBITCOMMON ${RABBITIM_DEFINES})
target_link_libraries(${PROJECT_NAME} PUBLIC RabbitCommon ${QT_LIBRARIES} ${RABBITIM_LIBS})
target_include_directories(${PROJECT_NAME}
    PRIVATE ${RabbitCommon_DIR}/Src
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/FrmCustom)

#if(BUILD_VERSION)
#    target_compile_definitions(${PROJECT_NAME} PRIVATE -DBUILD_VERSION="${BUILD_VERSION}")
#    #set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${BUILD_VERSION})
#endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE DLL_EXPORT)
GENERATE_EXPORT_HEADER(${PROJECT_NAME})
string(TOLOWER ${PROJECT_NAME} LOWER_PROJECT_NAME)
set(INSTALL_HEADER_FILES ${INSTALL_HEADER_FILES}
    ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h)
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h
    DESTINATION ${CMAKE_BINARY_DIR})

if(ANDROID)
    ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/android/QtAndroidUtils/android/QtAndroidUtilsModule/jni
        ${CMAKE_BINARY_DIR}/QtAndroidUtilsModule)
    target_link_libraries(${PROJECT_NAME} PUBLIC QtAndroidUtilsModule)
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/android/QtAndroidUtils/android/QtAndroidUtilsModule/jni)

    # Install target
    INSTALL(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Config
            RUNTIME DESTINATION "libs/${ANDROID_ABI}"
                COMPONENT Runtime
            LIBRARY DESTINATION "libs/${ANDROID_ABI}"
                COMPONENT Runtime
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/RabbitIm
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
else()
    # Install target
    if(WIN32)
        INSTALL(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Config
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
                COMPONENT Runtime
            LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}"
                COMPONENT Runtime
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/RabbitIm
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
    else()
        INSTALL(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Config
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
                COMPONENT Runtime
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
                COMPONENT Runtime
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/RabbitIm
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
    endif()
endif()

export(TARGETS ${PROJECT_NAME}
       APPEND FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
)
# Install cmake configure files
install(EXPORT ${PROJECT_NAME}Config
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
        )
write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${BUILD_VERSION}
    COMPATIBILITY AnyNewerVersion)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake")

IF( ("Release" STREQUAL CMAKE_BUILD_TYPE)
    OR ("MinSizeRel" STREQUAL CMAKE_BUILD_TYPE)
    AND WIN32 AND BUILD_SHARED_LIBS)
    IF( ("Release" STREQUAL CMAKE_BUILD_TYPE)
        OR ("MinSizeRel" STREQUAL CMAKE_BUILD_TYPE)
        AND MINGW)
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
    #INSTALL(DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>/" DESTINATION "${CMAKE_INSTALL_BINDIR}")
ENDIF()
