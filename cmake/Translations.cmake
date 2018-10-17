SET(TS_FILES
    Resource/translations/app_zh_CN.ts
    Resource/translations/app_zh_CN.ts
    Resource/translations/app_zh_TW.ts
    Resource/translations/app_cs.ts
    Resource/translations/app_da.ts
    Resource/translations/app_de.ts
    Resource/translations/app_fr.ts
    Resource/translations/app_hu.ts
    Resource/translations/app_ja.ts
    Resource/translations/app_ko.ts
    Resource/translations/app_pl.ts
    Resource/translations/app_ru.ts
    Resource/translations/app_sk.ts
    Resource/translations/app_sl.ts
    Resource/translations/app_uk.ts)
OPTION(OPTION_RABBITIM_TRANSLATIONS "Refresh translations on compile" ON)
MESSAGE("Refresh translations on compile: ${OPTION_RABBITIM_TRANSLATIONS}\n")
IF(OPTION_RABBITIM_TRANSLATIONS)
    FIND_PACKAGE(Qt5 CONFIG REQUIRED LinguistTools) #语言工具
    IF(NOT Qt5_LRELEASE_EXECUTABLE)
        MESSAGE(WARNING "Could not find lrelease. Your build won't contain translations.")
    ELSE(NOT Qt5_LRELEASE_EXECUTABLE)
        #qt5_create_translation(QM_FILES ${RABBITIM_SOURCES} ${RABBITIM_UIS} ${TS_FILES}) #生成 .ts 文件与 .qm 文件
        qt5_add_translation(QM_FILES ${TS_FILES}) #生成翻译资源文件
        ADD_CUSTOM_TARGET(translations ALL DEPENDS ${QM_FILES})
        add_dependencies(${PROJECT_NAME} translations)
        #调试时使用，复制到编译目录
        foreach(_file ${QM_FILES})
            IF(ANDROID)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        #COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/android-build/translations
                        COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${CMAKE_CURRENT_BINARY_DIR}/android-build/translations
                        COMMAND ${CMAKE_COMMAND} -E copy ${QT_INSTALL_DIR}/translations/qt_*.qm ${CMAKE_CURRENT_BINARY_DIR}/android-build/translations
                        )
            ELSE(ANDROID)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/translations
                        COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${CMAKE_CURRENT_BINARY_DIR}/translations
                        )
            ENDIF(ANDROID)
        endforeach()
        #IF(EXISTS "${QT_INSTALL_DIR}/translations/qt_*.qm" AND NOT ANDROID)
        #    FILE(COPY ${QT_INSTALL_DIR}/translations/qt_*.qm DESTINATION "translations")
        #ENDIF()

        #安装1:翻译
        INSTALL(DIRECTORY "${QT_INSTALL_DIR}/translations"
                DESTINATION "."
                FILES_MATCHING PATTERN "qt_*.qm")
        #INSTALL(FILES "${QM_FILES}" DESTINATION "translations" CONFIGURATIONS Release)
        INSTALL(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/translations"
                DESTINATION "."
                )
    ENDIF(NOT Qt5_LRELEASE_EXECUTABLE)
ENDIF(OPTION_RABBITIM_TRANSLATIONS)
