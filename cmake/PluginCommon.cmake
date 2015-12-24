#生成目标
IF(PLUGIN_SOURCES)
    #静态库或android系统,用翻译资源文件  
    IF(OPTION_RABBITIM_USE_STATIC OR ANDROID)
        IF(EXISTS ${PROJECT_SOURCE_DIR}/translations/Translations.qrc)
            SET(TRANSLATIONS_RESOURCE_FILES ${PROJECT_SOURCE_DIR}/translations/Translations.qrc)
        ENDIF() 
    ENDIF()
    IF(OPTION_RABBITIM_USE_STATIC)
        add_library(${PROJECT_NAME}
            ${PLUGIN_SOURCES}
            ${PLUGIN_UIS}
            ${TRANSLATIONS_RESOURCE_FILES}
            ${PLUGIN_RESOURCE_FILES}
            )
    ELSE(OPTION_RABBITIM_USE_STATIC)
        #windows下动态库导出
        ADD_DEFINITIONS(-DRABBITIM_SHARED_LIBRARY)
        add_library(${PROJECT_NAME} SHARED
            ${PLUGIN_SOURCES}
            ${PLUGIN_UIS}
            ${TRANSLATIONS_RESOURCE_FILES}
            ${PLUGIN_RESOURCE_FILES}
            )
    ENDIF(OPTION_RABBITIM_USE_STATIC)

    #链接库
    target_link_libraries(${PROJECT_NAME}
            ${RABBITIM_LIBS}
            RabbitIm
            )
ENDIF(PLUGIN_SOURCES)

include(${CMAKE_SOURCE_DIR}/cmake/TranslationsPlugin.cmake)

#为静态插件生成必要的文件  
IF(OPTION_RABBITIM_USE_STATIC)
        IF(NOT RABBITIM_PLUG_NAME) 
            message("Please set RABBITIM_PLUG_NAME to plug class name")
        ENDIF()
        SET(FILE_NAME ${CMAKE_SOURCE_DIR}/Plugin/PluginStatic.cpp)
        SET(PLUG_CONTENT "Q_IMPORT_PLUGIN(${RABBITIM_PLUG_NAME})")
        IF(EXISTS ${FILE_NAME})
            FILE(READ ${FILE_NAME} FILE_CONTENT)
            STRING(FIND ${FILE_CONTENT} ${PLUG_CONTENT} POSTION)
            if(POSTION EQUAL "-1")
                SET(PLUG_CONTENT "\nQ_IMPORT_PLUGIN(${RABBITIM_PLUG_NAME})")
                FILE(APPEND ${FILE_NAME} ${PLUG_CONTENT})
            ENDIF()
        ENDIF()

        SET(FILE_NAME ${CMAKE_SOURCE_DIR}/Plugin/PluginStatic.cmake)
        SET(PLUG_CONTENT  "${PROJECT_NAME}")
        IF(EXISTS ${FILE_NAME})
            FILE(READ ${FILE_NAME} FILE_CONTENT)
        ENDIF()
        STRING(FIND ${FILE_CONTENT} ${PLUG_CONTENT} POSTION)
        if(POSTION EQUAL "-1")
            SET(PLUG_CONTENT "\nSET(RABBITIM_LIBS \${RABBITIM_LIBS} ${PROJECT_NAME})")
            FILE(APPEND ${FILE_NAME} ${PLUG_CONTENT})
        ENDIF()
ELSE()
    #复制插件到 ${CMAKE_BINARY_DIR}/plugins/App/${PROJECT_NAME}
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/plugins/App/${PROJECT_NAME}"
            COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}" "${CMAKE_BINARY_DIR}/plugins/App/${PROJECT_NAME}/."
            )
ENDIF()

#安装
INSTALL(TARGETS ${PROJECT_NAME}
            LIBRARY DESTINATION "plugins/App/${PROJECT_NAME}"
            ARCHIVE DESTINATION "plugins/App/${PROJECT_NAME}")
