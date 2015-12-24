SET(TS_FILES
        translations/Plugin_zh_CN.ts
        translations/Plugin_zh_CN.ts
        translations/Plugin_zh_TW.ts
        translations/Plugin_cs.ts
        translations/Plugin_da.ts
        translations/Plugin_de.ts
        translations/Plugin_fr.ts
        translations/Plugin_hu.ts
        translations/Plugin_ja.ts
        translations/Plugin_ko.ts
        translations/Plugin_pl.ts
        translations/Plugin_ru.ts
        translations/Plugin_sk.ts
        translations/Plugin_sl.ts
        translations/Plugin_uk.ts)
OPTION(OPTION_RABBITIM_TRANSLATIONS "Refresh translations on compile" ON)
MESSAGE("Refresh translations on compile: ${OPTION_RABBITIM_TRANSLATIONS}\n")
IF(OPTION_RABBITIM_TRANSLATIONS)
    FIND_PACKAGE(Qt5 CONFIG REQUIRED LinguistTools) #语言工具
    IF(NOT Qt5_LRELEASE_EXECUTABLE)
        MESSAGE(WARNING "Could not find lrelease. Your build won't contain translations.")
    ELSE(NOT Qt5_LRELEASE_EXECUTABLE)
        qt5_add_translation(QM_FILES ${TS_FILES}) #生成翻译资源文件
        ADD_CUSTOM_TARGET(${PROJECT_NAME}_translations ALL DEPENDS ${QM_FILES})
        add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_translations)

        #静态库或android下生成翻译资源文件  
        IF(OPTION_RABBITIM_USE_STATIC)
                SET(TRANSLATIONS_RESOURCES_FILE ${PROJECT_SOURCE_DIR}/translations/Translations.qrc)
                #生成资源文件  
                #SET(TRANSLATIONS_RESOURCES_CONTENT "</qresource></RCC>")
                #FILE(READ ${TRANSLATIONS_RESOURCES_FILE} FILE_CONTENT)
                #STRING(FIND ${FILE_CONTENT} ${TRANSLATIONS_RESOURCES_CONTENT} POSTION)
                #if(POSTION EQUAL "-1")
                    SET(TRANSLATIONS_RESOURCES_CONTENT "<RCC><qresource prefix='/translations'>")
                    foreach(_file ${QM_FILES})
                        SET(TRANSLATIONS_RESOURCES_CONTENT ${TRANSLATIONS_RESOURCES_CONTENT} "\n<file>${_file}</file>")
                    endforeach()
                    SET(TRANSLATIONS_RESOURCES_CONTENT ${TRANSLATIONS_RESOURCES_CONTENT} "\n</qresource></RCC>")
                    FILE(WRITE ${TRANSLATIONS_RESOURCES_FILE} ${TRANSLATIONS_RESOURCES_CONTENT})
                #ENDIF()
        ELSE()
            #调试时使用，复制到编译目录
            foreach(_file ${QM_FILES})
                    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/plugins/App/${PROJECT_NAME}/translations
                            COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${CMAKE_BINARY_DIR}/plugins/App/${PROJECT_NAME}/translations
                            )
            endforeach()
            #IF(EXISTS "${QT_INSTALL_DIR}/translations/qt_*.qm" AND NOT ANDROID)
            #    FILE(COPY ${QT_INSTALL_DIR}/translations/qt_*.qm DESTINATION "translations")
            #ENDIF()
    
            #安装1:翻译
            INSTALL(DIRECTORY "${QT_INSTALL_DIR}/translations"
                    DESTINATION "."
                    FILES_MATCHING PATTERN "qt_*.qm")
            #INSTALL(FILES "${QM_FILES}" DESTINATION "translations" CONFIGURATIONS Release)
            INSTALL(DIRECTORY "${CMAKE_BINARY_DIR}/plugins/App/${PROJECT_NAME}/translations"
                    DESTINATION "plugins/App/${PROJECT_NAME}"
                    )
        ENDIF()

    ENDIF(NOT Qt5_LRELEASE_EXECUTABLE)
ENDIF(OPTION_RABBITIM_TRANSLATIONS)
