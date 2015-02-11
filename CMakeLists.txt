cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

if(POLICY CMP0010)
  cmake_policy(SET CMP0010 NEW)
endif()

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
    SET(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/RabbitIm CACHE STRING "Install path prefix.") 
ENDIF() 

# Find Git Version Patch
FIND_PROGRAM(GIT git)
IF(GIT)
    EXECUTE_PROCESS(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${GIT} rev-parse --short HEAD
        OUTPUT_VARIABLE BUILD_VERSION  OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    MESSAGE("Git Version: ${BUILD_VERSION}\n")
ENDIF(GIT)

MESSAGE("\n")

#增加pkgconfig模块
#include(FindPkgConfig)

#TODO:判断 qt 版本

FIND_PACKAGE(Qt5 CONFIG REQUIRED Gui Core Network Xml Multimedia Widgets LinguistTools)
SET(QT_INSTALL_DIR "${Qt5_DIR}/../../..")

include(cmake/RabbitImUtils.cmake)

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

SET(CMAKE_VERBOSE_MAKEFILE ON)
IF(NOT MSVC)
    SET(CMAKE_CXX_FLAGS "-std=c++0x") #启用C++11
ENDIF(NOT MSVC)

# Add definitions for static/style library 
OPTION(STATIC "Build statically" OFF)
MESSAGE("Build statically: ${STATIC}")
IF(STATIC)
    ADD_DEFINITIONS(-DQT_STATIC)
ELSE(STATIC)
    ADD_DEFINITIONS(-DQT_SHARED)
ENDIF(STATIC)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    ADD_DEFINITIONS(-DDEBUG)# -DDEBUG_VIDEO_TIME )
ENDIF()

#设置头文件包含位置 
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
IF(MSVC)
    FIND_PACKAGE(Qt5 CONFIG REQUIRED WebKitWidgets)
    SET(RABBITIM_SYSTEM windows)
    SET(TOOLCHAIN_NAME windows_msvc)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmtd /NODEFAULTLIB:libcmt")
ELSEIF(MINGW)
    FIND_PACKAGE(Qt5 CONFIG REQUIRED WebKitWidgets)
    SET(RABBITIM_SYSTEM windows)
    SET(TOOLCHAIN_NAME windows_mingw)
    # Windows compatibility
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,windows")
    # Statically link with libgcc
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ ")
ELSEIF(ANDROID)
    SET(RABBITIM_SYSTEM android)
    SET(TOOLCHAIN_NAME android)
    ADD_DEFINITIONS(-DMOBILE)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/android/jni)
    FIND_PACKAGE(Qt5 CONFIG REQUIRED  AndroidExtras)
    #SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wno-psabi -march=armv7-a -mfloat-abi=softfp -mfpu=vfp -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums  -Wa,--noexecstack -gdwarf-2 -marm -fno-omit-frame-pointer -Wall -Wno-psabi -W -fPIE)
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined -Wl,-z,noexecstack -shared --sysroot=${ANDROID_SYSROOT}")
ELSE(LINUX OR UNIX)
    FIND_PACKAGE(Qt5 CONFIG REQUIRED WebKitWidgets)
    SET(RABBITIM_SYSTEM unix)
    ADD_DEFINITIONS(-DUNIX)
    SET(TOOLCHAIN_NAME unix)
ENDIF()
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} 
    ${CMAKE_SOURCE_DIR}/Widgets/FrmCustom
    ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/include)

SET(THIRD_PACKAGE ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME})   #第三方开发包目录  
SET(THIRD_LIB_DIR ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/lib) #第三方库目录
LINK_DIRECTORIES(${THIRD_LIB_DIR})
#设置库的搜索路径  
SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME})

# 依赖的第三方库
OPTION(OPTION_USE_VPX "Use vpx library" ON)
MESSAGE("Use vpx library:${USE_VPX}\n")
IF(OPTION_USE_VPX)
    SET(USE_VPX ON)
    SET(VPX_LIBS vpx)
    IF(ANDROID)
        SET(VPX_LIBS ${VPX_LIBS} cpu-features)
    ENDIF(ANDROID)
ENDIF(OPTION_USE_VPX)

OPTION(USE_SPEEX "Use speex library" OFF)
MESSAGE("Use speex library:${USE_SPEEX}\n")
IF(USE_SPEEX)
    SET(SPEEX_LIBS speex)
ENDIF()

#TODO:增加检测ffmpeg库
OPTION(OPTION_RABBITIM_USER_FFMPEG "Use ffmpeg library" ON)
IF(OPTION_RABBITIM_USER_FFMPEG)
    SET(RABBITIM_USER_FFMPEG ON)
    IF(RABBITIM_USER_FFMPEG)
        ADD_DEFINITIONS("-D__STDC_CONSTANT_MACROS" "-DRABBITIM_USER_FFMPEG") #ffmpeg需要
        IF(MSVC)
            SET(FFMPEG_LIBS 
                ${THIRD_LIB_DIR}/libavcodec.a 
                ${THIRD_LIB_DIR}/libavformat.a 
                ${THIRD_LIB_DIR}/libswscale.a 
                ${THIRD_LIB_DIR}/libavfilter.a 
                ${THIRD_LIB_DIR}/libavutil.a)
        ELSE()
            SET(FFMPEG_LIBS avcodec avformat swscale avfilter avutil)
        ENDIF()
    ENDIF(RABBITIM_USER_FFMPEG)
ENDIF(OPTION_RABBITIM_USER_FFMPEG)
MESSAGE("Use ffmpeg library:${RABBITIM_USER_FFMPEG}\n")

#TODO:增加检测opencv库
OPTION(OPTION_RABBITIM_USER_OPENCV "Use opencv library" OFF)
IF(OPTION_RABBITIM_USER_OPENCV)
    IF(RABBITIM_USER_OPENCV)
        add_definitions("-DRABBITIM_USER_OPENCV")
        SET(RABBITIM_USER_OPENCV ON)
        IF(WIN32)
            SET(OPENCV_VERSION 300)
        ENDIF(WIN32)
        SET(OPENCV_LIBS 
            opencv_core${OPENCV_VERSION}
            opencv_imgproc${OPENCV_VERSION})
    ENDIF(RABBITIM_USER_OPENCV)
ENDIF(OPTION_RABBITIM_USER_OPENCV)
MESSAGE("Use opencv library:${RABBITIM_USER_OPENCV}\n")

#检测libcurl库
OPTION(RABBITIM_USER_LIBCURL "Use curl library" ON)
IF(RABBITIM_USER_LIBCURL)
    add_definitions("-DRABBITIM_USER_LIBCURL")
    IF(MSVC)
        SET(CURL_LIBRARY -llibcurl)
    ELSEIF(MINGW)
        add_definitions("-DCURL_STATICLIB")
        SET(CURL_LIBRARY -lcurl -lcrypto -lssl -lgdi32 -lwldap32 -lz -lws2_32)
    ELSE(MSVC)
        find_package(CURL)
        if(NOT CURL_FOUND)
            MESSAGE("Don't find libcurl library\n")
        ENDIF(NOT CURL_FOUND)
    ENDIF(MSVC)
    SET(RABBITIM_USER_LIBCURL ON)

    include_directories(${CURL_INCLUDE_DIR})
    SET(LIBCURL_LIBS ${CURL_LIBRARY})
ENDIF(RABBITIM_USER_LIBCURL)
MESSAGE("Use curl library:${RABBITIM_USER_LIBCURL}\n")

#检测openssl库
OPTION(OPTRABBITIM_USER_OPENSSL "Use openssl library" ON)
IF(OPTRABBITIM_USER_OPENSSL)
    add_definitions("-DRABBITIM_USER_OPENSSL")
    SET(RABBITIM_USER_OPENSSL ON)
    IF(MSVC)
        SET(OPENSSL_LIBRARIES -llibeay32 -lssleay32)
    ELSEIF(MINGW)
        SET(OPENSSL_LIBRARIES -lcrypto -lssl)
    ELSE(MSVC)
        find_package(OpenSSL)
        IF(NOT OPENSSL_FOUND)
            MESSAGE("Don't find openssl library\n")
        ENDIF()
    ENDIF(MSVC)

    include_directories(${OPENSSL_INCLUDE_DIR})
    SET(OPENSSL_LIBS ${OPENSSL_LIBRARIES})
ENDIF(OPTRABBITIM_USER_OPENSSL)
MESSAGE("Use openssl library:${RABBITIM_USER_OPENSSL}\n")

#连接静态QXMPP库时，必须加上-DQXMPP_STATIC。生成静态QXMPP库时，qmake 需要加上 QXMPP_LIBRARY_TYPE=staticlib 参数
ADD_DEFINITIONS("-DQXMPP") # -DQXMPP_STATIC")
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    SET(QXMPP_LIBS qxmpp_d0)
ELSE()
    SET(QXMPP_LIBS qxmpp0)
ENDIF()
message("QXMPP_LIBS:${QXMPP_LIBS}")

SET(RABBITIM_LIBS
    ${RABBITIM_LIBS}
    ${QXMPP_LIBS}
    ${OPENCV_LIBS}
    ${FFMPEG_LIBS}
    ${VPX_LIBS}
    ${SPEEX_LIBS}
    ${LIBCURL_LIBS}
    ${OPENSSL_LIBS}
    Qt5::Widgets
    Qt5::Network
    Qt5::Multimedia
    Qt5::Xml
    Qt5::Core)

include(cmake/install_version.cmake)
include(cmake/cmake_build_file.cmake)

SET(RABBITIM_RCC_FILES Resource/Resource.qrc Resource/sink/dark/style.qrc  AppIcon.rc)

#生成目标
IF(ANDROID)
    add_library(${PROJECT_NAME} SHARED 
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        )
    SET(RABBITIM_LIBS
        ${RABBITIM_LIBS} 
        Qt5::AndroidExtras)
ELSEIF(WIN32)
    add_executable(${PROJECT_NAME} WIN32
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        ${RABBITIM_UIS}
        )
    SET(RABBITIM_LIBS
        ${RABBITIM_LIBS} 
        Qt5::WebKitWidgets)
ELSE()
    add_executable(${PROJECT_NAME} 
        ${RABBITIM_SOURCES}
        ${RABBITIM_RCC_FILES}
        ${RABBITIM_UIS}
        )
    SET(RABBITIM_LIBS
        ${RABBITIM_LIBS} 
        Qt5::WebKitWidgets)
ENDIF()
message("RABBITIM_LIBS:${RABBITIM_LIBS}\n")
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
                        #COMMAND ${CMAKE_COMMAND} -E make_directory  ${PROJECT_BINARY_DIR}/android-build/translations
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
        FILE(COPY ${QT_INSTALL_DIR}/translations/qt_zh_CN.qm DESTINATION "translations")

        #安装1:翻译
        INSTALL(FILES "${QT_INSTALL_DIR}/translations/qt_zh_CN.qm" DESTINATION "translations" CONFIGURATIONS Release)
        INSTALL(FILES "${QM_FILES}" DESTINATION "translations" CONFIGURATIONS Release)
    ENDIF(NOT Qt5_LRELEASE_EXECUTABLE)
ENDIF(RABBITIM_TRANSLATIONS)

GENERATED_DEPLOYMENT_SETTINGS()
find_program(ANT NAMES ant) # PATHS  "/usr/bin")
message("ant:${ANT}\n")

#只发行版本才安装
IF(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    #安装2:目标
    IF(ANDROID)
        INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}")
    ELSE(ANDROID)
        INSTALL(TARGETS "${PROJECT_NAME}" RUNTIME DESTINATION "." CONFIGURATIONS Release)
    ENDIF(ANDROID)

    #安装3:qt库
    IF(WIN32)
        #add_custom_target(Deployment DEPENDS ${PROJECT_NAME}
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/${PROJECT_NAME}"
            COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.exe" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/."
            COMMAND "${QT_INSTALL_DIR}/bin/windeployqt"
                --compiler-runtime
                --no-translations
                --verbose 7
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}.exe"
        )
        install(DIRECTORY "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/." DESTINATION "." CONFIGURATIONS Release)
    ELSE(WIN32)
        SET(QT_LIBS Qt5::Widgets Qt5::Network Qt5::Multimedia Qt5::Core Qt5::Gui Qt5::Xml Qt5::WebKitWidgets)
        INSTALL_QT_LIBRARYS(${QT_LIBS})
        INSTALL_QT_PLUGINS(Qt5Gui_PLUGINS Qt5Network_PLUGINS Qt5Widgets_PLUGINS Qt5Xml_PLUGINS)
    ENDIF(WIN32)
ENDIF(NOT CMAKE_BUILD_TYPE MATCHES Debug)

IF(ANDROID)
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

#复制第三方依赖库的动态库到编译目录下
IF(WIN32)
    file(GLOB THIRD_DLL_LIBRARY "${CMAKE_SOURCE_DIR}/ThirdLibary/${TOOLCHAIN_NAME}/bin/*.dll")
    message("THIRD_DLL_LIBRARY:${THIRD_DLL_LIBRARY}")
    FOREACH(file ${THIRD_DLL_LIBRARY})
        IF(EXISTS "${file}")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy "${file}" "${PROJECT_BINARY_DIR}/."
            )
            #安装4:第三方动态库
            INSTALL(FILES ${file} DESTINATION . CONFIGURATIONS Release)
        ENDIF(EXISTS "${file}")
    ENDFOREACH(file)

ENDIF(WIN32)

#安装5:安装相关文件
install(FILES License.html Authors.txt ChangeLog.txt DESTINATION .)
