SET(LOCALE_NAME
    zh_CN
    zh_TW
    cs
    da
    de
    fr
    hu
    ja
    ko
    pl
    ru
    sk
    sl
    uk)

foreach(_current_NAME ${LOCALE_NAME})
    SET(TS_FILES ${TS_FILES} Resource/translations/app_${_current_NAME}.ts)
endforeach()
SET(RABBITIM_RCC_FILES ${RABBITIM_RCC_FILES} ${TS_FILES})

OPTION(OPTION_RABBITIM_TRANSLATIONS "Refresh translations on compile" ON)
MESSAGE("Refresh translations on compile: ${OPTION_RABBITIM_TRANSLATIONS}\n")
IF(OPTION_RABBITIM_TRANSLATIONS)
    FIND_PACKAGE(Qt5 CONFIG REQUIRED LinguistTools) #语言工具
    IF(NOT Qt5_LRELEASE_EXECUTABLE)
        MESSAGE(WARNING "Could not find lrelease. Your build won't contain translations.")
    ELSE(NOT Qt5_LRELEASE_EXECUTABLE)
        #qt5_create_translation(QM_FILES ${RABBITIM_SOURCES} ${RABBITIM_UIS} ${TS_FILES}) #生成 .ts 文件与 .qm 文件
        #qt5_add_translation(QM_FILES ${TS_FILES}) #生成翻译资源文件
        
        SET(TRANSLATION_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/translations")
        if(NOT EXISTS "${TRANSLATION_OUTPUT_DIRECTORY}")
            file(MAKE_DIRECTORY "${TRANSLATION_OUTPUT_DIRECTORY}")
        endif()
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
        
        SET(RABBITIM_RCC_FILES ${RABBITIM_RCC_FILES} ${QM_FILES})

        if(ANDROID)
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
          
            SET(RABBITIM_RCC_FILES
                ${RABBITIM_RCC_FILES} 
                ${TRANSLATION_OUTPUT_DIRECTORY}/translations.qrc)
        else(ANDROID)
            #安装翻译
            INSTALL(DIRECTORY "${QT_INSTALL_DIR}/translations"
                    DESTINATION "."
                    FILES_MATCHING PATTERN "qt_*.qm")
            #INSTALL(FILES "${QM_FILES}" DESTINATION "translations" CONFIGURATIONS Release)
            
            INSTALL(DIRECTORY "${TRANSLATION_OUTPUT_DIRECTORY}"
                    DESTINATION "."
                    FILES_MATCHING PATTERN "*.qm")
        endif(ANDROID)
    ENDIF(NOT Qt5_LRELEASE_EXECUTABLE)
ENDIF(OPTION_RABBITIM_TRANSLATIONS)
