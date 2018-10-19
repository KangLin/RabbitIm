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
        #qt5_add_translation(QM_FILES ${TS_FILES}) #生成翻译资源文件
        #ADD_CUSTOM_TARGET(${PROJECT_NAME}_translations ALL DEPENDS ${QM_FILES})
        #add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_translations)

        SET(TRANSLATION_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/${PLUGIN_TYPE}/${PROJECT_NAME}/translations")
        file(MAKE_DIRECTORY "${TRANSLATION_OUTPUT_DIRECTORY}")
        set(QM_FILES)
        foreach(_current_FILE ${TS_FILES})
            get_filename_component(_abs_FILE ${_current_FILE} ABSOLUTE)
            get_filename_component(qm_file ${_abs_FILE} NAME_WE)
            set(qm_file ${TRANSLATION_OUTPUT_DIRECTORY}/${qm_file}.qm)
  
            add_custom_command(OUTPUT ${qm_file}
                COMMAND ${Qt5_LRELEASE_EXECUTABLE}
                ARGS ${_abs_FILE} -qm ${qm_file}         
                )
            set(QM_FILES ${QM_FILES} ${qm_file})
        endforeach()
        
        SET(PLUGIN_RESOURCE_FILES ${PLUGIN_RESOURCE_FILES} ${QM_FILES})
        
        #静态库或android下生成翻译资源文件  
        IF(NOT BUILD_SHARED_LIBS OR ANDROID)
            file(WRITE "${TRANSLATION_OUTPUT_DIRECTORY}/translations.qrc.in"
            "<!DOCTYPE RCC>
            <RCC version=\"1.0\">
              <qresource prefix=\"/translations\">
            ")
            foreach(qm ${QM_FILES})
              get_filename_component(qm_name ${qm} NAME_WE)
              file(APPEND "${TRANSLATION_OUTPUT_DIRECTORY}/translations.qrc.in"
                "    <file alias=\"${qm_name}\">${qm}</file>\n")
            endforeach(qm)
            file(APPEND "${TRANSLATION_OUTPUT_DIRECTORY}/translations.qrc.in"
            "  </qresource>
            </RCC>
            ")
            
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${TRANSLATION_OUTPUT_DIRECTORY}/translations.qrc.in
              ${TRANSLATION_OUTPUT_DIRECTORY}/translations.qrc)
          
            SET(PLUGIN_RESOURCE_FILES
                ${PLUGIN_RESOURCE_FILES} 
                ${TRANSLATION_OUTPUT_DIRECTORY}/translations.qrc)
        ELSE()
            #安装
            #INSTALL(FILES "${QM_FILES}" DESTINATION "translations" CONFIGURATIONS Release)
            INSTALL(DIRECTORY "${TRANSLATION_OUTPUT_DIRECTORY}"
                    DESTINATION "plugins/App/${PROJECT_NAME}"
                    FILES_MATCHING PATTERN "*.qm")
        ENDIF()

    ENDIF(NOT Qt5_LRELEASE_EXECUTABLE)
ENDIF(OPTION_RABBITIM_TRANSLATIONS)
