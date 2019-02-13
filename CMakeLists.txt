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
SET(BUILD_VERSION "v0.1.0-312-gbbf45f4")
# Find Git Version Patch
IF(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    if(NOT GIT)
        SET(GIT $ENV{GIT})
    endif()
    if(NOT GIT)
        FIND_PROGRAM(GIT NAMES git git.exe git.cmd)
    endif()
    IF(GIT)
        EXECUTE_PROCESS(
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND ${GIT} describe --tags
            OUTPUT_VARIABLE GIT_VERSION  OUTPUT_STRIP_TRAILING_WHITESPACE
        )
	    if(NOT GIT_VERSION)
            EXECUTE_PROCESS(
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMAND ${GIT} rev-parse --short HEAD
                OUTPUT_VARIABLE GIT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE
            )
        endif()
        SET(BUILD_VERSION ${GIT_VERSION})
    ENDIF(GIT)
ENDIF()
message("BUILD_VERSION:${BUILD_VERSION}")

add_definitions(-DBUILD_VERSION="${BUILD_VERSION}")

include(cmake/Configure.cmake)
include(cmake/ThirdLibrary.cmake)
include(cmake/install_version.cmake)
include(cmake/cmake_build_file.cmake)
add_subdirectory(Plugin)
IF(NOT BUILD_SHARED_LIBS)
    include(Plugin/PluginStatic.cmake)
ENDIF()

#翻译
include(cmake/Translations.cmake)

#设置资源文件
SET(RABBITIM_RCC_FILES
    ${RABBITIM_RCC_FILES}
    Resource/Resource.qrc
    Resource/sink/dark/style.qrc
    )

#生成目标
add_library(${PROJECT_NAME} 
    ${RABBITIM_SOURCES}
    ${RABBITIM_RCC_FILES}
    ${RABBITIM_UIS}
    )
IF(BUILD_SHARED_LIBS)
    #windows下动态库导出
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DBUILD_SHARED_LIBS) 
    if(WIN32)
        target_compile_definitions(${PROJECT_NAME} PRIVATE -DDLL_EXPORT)
    endif(WIN32)
ENDIF()
message("RABBITIM_LIBS:${RABBITIM_LIBS}")
#链接库
target_link_libraries(${PROJECT_NAME}
        ${RABBITIM_LIBS}
        )
IF(NOT ANDROID)
    add_executable(${PROJECT_NAME}App
        main.cpp
        AppIcon.rc
        )
    target_link_libraries(${PROJECT_NAME}App
            ${PROJECT_NAME}
            ${RABBITIM_LIBS}
            )
        
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
    set_target_properties(${PROJECT_NAME}App PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
ENDIF()

#只发行版本才安装
IF(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    
    #安装1:目标
    if(BUILD_SHARED_LIBS)
        INSTALL(TARGETS ${PROJECT_NAME}
            LIBRARY DESTINATION "."
            ARCHIVE DESTINATION ".")
    endif()
    IF(NOT ANDROID)
        INSTALL(TARGETS "${PROJECT_NAME}App" RUNTIME DESTINATION "." CONFIGURATIONS Release)
    ENDIF()
    if(MSVC)
        SET(VAR_RABBITIM_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
        SET(VAR_RABBITIMAPP_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    else()
        SET(VAR_RABBITIM_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
        SET(VAR_RABBITIMAPP_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    endif()
    #安装2:qt库
    IF(WIN32 AND CMAKE_HOST_WIN32)
        IF(MINGW)
            #windeployqt 分发时，是根据是否 strip 来判断是否是 DEBUG 版本,而用mingw编译时,qt没有自动 strip
            if(BUILD_SHARED_LIBS)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND strip "${VAR_RABBITIM_OUTPUT_DIRECTORY}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}"
                    )
            endif()
            add_custom_command(TARGET ${PROJECT_NAME}App POST_BUILD
                COMMAND strip "${VAR_RABBITIMAPP_OUTPUT_DIRECTORY}/${PROJECT_NAME}App${CMAKE_EXECUTABLE_SUFFIX}"
                )
        ENDIF()

        if(NOT QT_INSTALL_DIR)
            SET(QT_INSTALL_DIR "${Qt5_DIR}/../../.." CACHE PATH "" FORCE)
        endif()
        #add_custom_target(Deployment DEPENDS ${PROJECT_NAME}
        #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
        if(NOT EXISTS "${PROJECT_BINARY_DIR}/${PROJECT_NAME}")
            file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/${PROJECT_NAME}")
        endif()

        if(BUILD_SHARED_LIBS)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy "${VAR_RABBITIM_OUTPUT_DIRECTORY}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/."
                COMMAND "${QT_INSTALL_DIR}/bin/windeployqt"
                    --compiler-runtime
                    --verbose 7
                    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}"
            )
        endif()
        add_custom_command(TARGET ${PROJECT_NAME}App POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${VAR_RABBITIMAPP_OUTPUT_DIRECTORY}/${PROJECT_NAME}App${CMAKE_EXECUTABLE_SUFFIX}" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/."
            COMMAND "${QT_INSTALL_DIR}/bin/windeployqt"
                --compiler-runtime
                --verbose 7
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}App${CMAKE_EXECUTABLE_SUFFIX}"
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

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJECT_BINARY_DIR}/libRabbitIm.so ${PROJECT_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/libRabbitIm.so
        #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
        COMMAND "${QT_INSTALL_DIR}/bin/androiddeployqt"
            --input ${PROJECT_BINARY_DIR}/android-libRabbitIm.so-deployment-settings.json
            --output ${PROJECT_BINARY_DIR}/android-build
            --verbose
            #--ant /usr/bin/ant
            #--jdk $ENV{JAVA_HOME}
    )
ENDIF(ANDROID)

#安装3:第三方动态库
IF(WIN32)
    INSTALL(DIRECTORY "${THIRD_LIBRARY_PATH}/bin/" "${THIRD_LIBRARY_PATH}/lib/"
            DESTINATION .
            FILES_MATCHING PATTERN "*.dll")
    
    #复制第三方依赖库的动态库到编译目录下
    FILE(COPY "${THIRD_LIBRARY_PATH}/bin/" "${THIRD_LIBRARY_PATH}/lib/"
        DESTINATION .
        FILES_MATCHING PATTERN "*.dll")
ELSEIF(NOT ANDROID)
    INSTALL(DIRECTORY "${THIRD_LIBRARY_PATH}/lib/"
            DESTINATION "."
            FILES_MATCHING PATTERN "*.so*")
ENDIF(WIN32)

#SET(CMAKE_INSTALL_RPATH ".") #设置链接库的搜索路径

#安装4:安装相关文件
install(FILES License.md Authors.txt ChangeLog.md DESTINATION .)
