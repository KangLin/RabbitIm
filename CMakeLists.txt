cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(RabbitIm)

# 建立要删除的文件
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
# 建立删除目标
ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")

# 设置安装前缀
IF(NOT CMAKE_INSTALL_PREFIX) 
    SET(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/${PROJECT_NAME} CACHE STRING "Install path prefix.") 
ENDIF() 

# Find Git Version Patch
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    FIND_PROGRAM(GIT git)
    IF(GIT)
        EXECUTE_PROCESS(
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND ${GIT} rev-parse --short HEAD
            OUTPUT_VARIABLE BUILD_VERSION  OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        MESSAGE("Git Version: ${BUILD_VERSION}\n")
    ENDIF(GIT)
ENDIF()
MESSAGE("\n")

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

include(cmake/Configure.cmake)
include(cmake/ThirdLibrary.cmake)
include(cmake/install_version.cmake)
include(cmake/cmake_build_file.cmake)
include(Plugin/CMakeLists.txt)

SET(RABBITIM_LIBS
    ${RABBITIM_LIBS}
    ${QXMPP_LIBRARIES}
    ${LIBQRENCODE_LIBRARIES}
    ${QZXING_LIBRARIES}
    ${OpenCV_LIBS}
    ${FFMPEG_LIBRARIES}
    ${VPX_LIBRARIES}
    ${SPEEX_LIBRARIES}
    ${CURL_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${QT_LIBRARIES}
    )

#设置资源文件
SET(RABBITIM_RCC_FILES
    ${RABBITIM_RCC_FILES}
    Resource/Resource.qrc
    Resource/sink/dark/style.qrc
    AppIcon.rc)

#生成目标
IF(WIN32)
    add_executable(${PROJECT_NAME} WIN32
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        ${RABBITIM_UIS}
        )
ELSEIF(ANDROID)
    add_library(${PROJECT_NAME} SHARED 
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        ${RABBITIM_UIS}
        )
    #qt5_wrap_ui(RABBITIM_UIS ${RABBITIM_UIS})
ELSE()
    add_executable(${PROJECT_NAME} 
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        ${RABBITIM_UIS}
        )
ENDIF()
message("RABBITIM_LIBS:${RABBITIM_LIBS}")
#链接库
target_link_libraries(${PROJECT_NAME} 
        ${RABBITIM_LIBS}
        )

#翻译
SET(TS_FILES Resource/translations/app_zh_CN.ts)
OPTION(RABBITIM_TRANSLATIONS "Refresh translations on compile" ON)
MESSAGE("Refresh translations on compile: ${RABBITIM_TRANSLATIONS}\n")
IF(RABBITIM_TRANSLATIONS)
    IF(NOT Qt5_LRELEASE_EXECUTABLE)
        MESSAGE(WARNING "Could not find lrelease. Your build won't contain translations.")
    ELSE(NOT Qt5_LRELEASE_EXECUTABLE)
        qt5_add_translation(QM_FILES ${TS_FILES}) #生成翻译资源文件  
        ADD_CUSTOM_TARGET(translations ALL DEPENDS ${QM_FILES})
        add_dependencies(${PROJECT_NAME} translations)
        foreach(_file ${QM_FILES})
            IF(ANDROID)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        #COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/android-build/translations
                        COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${PROJECT_BINARY_DIR}/android-build/translations
                        COMMAND ${CMAKE_COMMAND} -E copy ${QT_INSTALL_DIR}/translations/qt_zh_CN.qm ${PROJECT_BINARY_DIR}/android-build/translations
                        )
            ELSE(ANDROID)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E make_directory translations
                        COMMAND ${CMAKE_COMMAND} -E copy ${_file} translations
                        )
            ENDIF(ANDROID)
        endforeach()
        IF(EXISTS "${QT_INSTALL_DIR}/translations/qt_zh_CN.qm")
            FILE(COPY ${QT_INSTALL_DIR}/translations/qt_zh_CN.qm DESTINATION "translations")
        ENDIF()

        #安装1:翻译
        INSTALL(FILES "${QT_INSTALL_DIR}/translations/qt_zh_CN.qm" DESTINATION "translations" CONFIGURATIONS Release)
        INSTALL(FILES "${QM_FILES}" DESTINATION "translations" CONFIGURATIONS Release)
    ENDIF(NOT Qt5_LRELEASE_EXECUTABLE)
ENDIF(RABBITIM_TRANSLATIONS)

#只发行版本才安装
IF(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    #安装2:目标
    IF(ANDROID)
        INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}")
    ELSE(ANDROID)
        INSTALL(TARGETS "${PROJECT_NAME}" RUNTIME DESTINATION "." CONFIGURATIONS Release)
    ENDIF(ANDROID)

    #安装3:qt库
    IF(WIN32 AND CMAKE_HOST_WIN32)
        IF(MINGW)
            #windeployqt 分发时，是根据是否 strip 来判断是否是 DEBUG 版本,而用mingw编译时,qt没有自动 strip
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND strip "${PROJECT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
                )
        ENDIF()

        #add_custom_target(Deployment DEPENDS ${PROJECT_NAME}
        #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/${PROJECT_NAME}"
            COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/."
            COMMAND "${QT_INSTALL_DIR}/bin/windeployqt"
                --compiler-runtime
                --verbose 7
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
        )
        INSTALL(DIRECTORY "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/." DESTINATION ".")
        #install(CODE "execute_process(COMMAND \"${QT_INSTALL_DIR}/bin/windeployqt\"
        #                    --compiler-runtime
        #                    --no-translations
        #                    --verbose 7
        #                    \"${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}\"
        #                    WORKING_DIRECTORY .)"
        #        )
    ELSEIF(NOT ANDROID)
        SET(QT_PLUGINS ${QT_PLUGINS} Qt5Core_PLUGINS 
                       Qt5Gui_PLUGINS
                       Qt5Widgets_PLUGINS
                       Qt5Network_PLUGINS
                       Qt5Multimedia_PLUGINS
                       Qt5Xml_PLUGINS)
        IF(Qt5WebKitWidgets_FOUND)
            SET(QT_PLUGINS ${QT_PLUGINS} Qt5WebKitWidgets_PLUGINS)
        ENDIF(Qt5WebKitWidgets_FOUND)
        INSTALL_QT_LIBRARYS(${QT_LIBRARIES})
        INSTALL_QT_PLUGINS(${QT_PLUGINS})
    ENDIF()
ENDIF(NOT CMAKE_BUILD_TYPE MATCHES Debug)

IF(ANDROID)
    GENERATED_DEPLOYMENT_SETTINGS()
    find_program(ANT NAMES ant) # PATHS  "/usr/bin")
    MESSAGE("ant:${ANT}\n")

    add_custom_command(TARGET  ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/libRabbitIm.so ${PROJECT_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/libRabbitIm.so
        #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
        COMMAND "${QT_INSTALL_DIR}/bin/androiddeployqt"    
            --input ${PROJECT_BINARY_DIR}/android-libRabbitIm.so-deployment-settings.json
            --output ${PROJECT_BINARY_DIR}/android-build
            --verbose
            #--ant /usr/bin/ant
            #--jdk $ENV{JAVA_HOME}
    )
ENDIF(ANDROID)

SET(CMAKE_INSTALL_RPATH ".") #设置链接库的搜索路径
#复制第三方依赖库的动态库到编译目录下
IF(WIN32)
    FILE(GLOB THIRD_DLL_LIBRARY_BIN "${CMAKE_SOURCE_DIR}/ThirdLibrary/${TOOLCHAIN_NAME}/bin/*.dll")
    FILE(GLOB THIRD_DLL_LIBRARY_LIB "${CMAKE_SOURCE_DIR}/ThirdLibrary/${TOOLCHAIN_NAME}/lib/*.dll")
    SET(THIRD_DLL_LIBRARY "${THIRD_DLL_LIBRARY_BIN} ${THIRD_DLL_LIBRARY_LIB}")
    #安装4:第三方动态库
    INSTALL(DIRECTORY "${CMAKE_SOURCE_DIR}/ThirdLibrary/${TOOLCHAIN_NAME}/bin/" 
            DESTINATION .
            FILES_MATCHING PATTERN "*.dll")
ELSEIF(NOT ANDROID)
    FILE(GLOB THIRD_DLL_LIBRARY "${CMAKE_SOURCE_DIR}/ThirdLibrary/${TOOLCHAIN_NAME}/lib/*.so")
    #安装4:第三方动态库
    INSTALL(DIRECTORY "${CMAKE_SOURCE_DIR}/ThirdLibrary/${TOOLCHAIN_NAME}/lib/"
            DESTINATION "."
            FILES_MATCHING PATTERN "*.so*")
ENDIF(WIN32)
#MESSAGE("THIRD_DLL_LIBRARY:${THIRD_DLL_LIBRARY}")
FOREACH(file ${THIRD_DLL_LIBRARY})
    IF(EXISTS "${file}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${file}" "${PROJECT_BINARY_DIR}/."
            )
     ENDIF(EXISTS "${file}")
ENDFOREACH(file)

#安装5:安装相关文件
install(FILES License.md Authors.txt ChangeLog.md DESTINATION .)
