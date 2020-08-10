
##为静态插件生成必要的文件
#    IF(NOT RABBITIM_PLUG_NAME)
#        message("Please set RABBITIM_PLUG_NAME to plug class name")
#    ENDIF()
#    SET(FILE_NAME ${CMAKE_SOURCE_DIR}/Plugin/PluginStatic.cpp)
#    SET(PLUG_CONTENT "Q_IMPORT_PLUGIN(${RABBITIM_PLUG_NAME})")
#    IF(EXISTS ${FILE_NAME})
#        FILE(READ ${FILE_NAME} FILE_CONTENT)
#        STRING(FIND ${FILE_CONTENT} ${PLUG_CONTENT} POSTION)
#        if(POSTION EQUAL "-1")
#            SET(PLUG_CONTENT "\nQ_IMPORT_PLUGIN(${RABBITIM_PLUG_NAME})")
#            FILE(APPEND ${FILE_NAME} ${PLUG_CONTENT})
#        ENDIF()
#    ENDIF()
    
#    SET(FILE_NAME ${CMAKE_SOURCE_DIR}/Plugin/PluginStatic.cmake)
#    SET(PLUG_CONTENT  "${PROJECT_NAME}")
#    IF(EXISTS ${FILE_NAME})
#        FILE(READ ${FILE_NAME} FILE_CONTENT)
#    ENDIF()
#    STRING(FIND ${FILE_CONTENT} ${PLUG_CONTENT} POSTION)
#    if(POSTION EQUAL "-1")
#        SET(PLUG_CONTENT "\nSET(RABBITIM_LIBS \${RABBITIM_LIBS} ${PROJECT_NAME})")
#        FILE(APPEND ${FILE_NAME} ${PLUG_CONTENT})
#    ENDIF()


function(ADD_PLUGIN_TARGET)
    SET(MUT_PARAS
        SOURCE_FILES            #源文件（包括头文件， ui 文件等）
        RESOURCE_FILES          #资源文件
        INCLUDE_DIRS            #包含目录
        LIBS                    #公有依赖库
        PRIVATE_LIBS            #私有依赖库
        DEFINITIONS             #公有宏定义
        PRIVATE_DEFINITIONS     #私有宏宏义
        OPTIONS                 #公有选项
        PRIVATE_OPTIONS         #私有选项
        FEATURES                #公有特性
        PRIVATE_FEATURES        #私有特性
        )
    cmake_parse_arguments(PARA ""
        "NAME;VERSION;PLUGIN_TYPE"
        "${MUT_PARAS}"
        ${ARGN})
    if(NOT DEFINED PARA_SOURCE_FILES)
        message(FATAL_ERROR "Use:
            ADD_TARGET
                [NAME name]
                [PLUGIN_TYPE type]
                SOURCE_FILES source1 [source2 ... header1 ... ui1 ...]
                [RESOURCE_FILES resource1 [resource2 ...]
                [LIBS lib1 [lib2 ...]]
                [PRIVATE_LIBS lib1 [lib2 ...]]
                [INCLUDE_DIRS [include_dir1 ...]]
                [DEFINITIONS [definition1 ...]]
                [PRIVATE_DEFINITIONS [defnitions1 ...]]
                [PRIVATE_OPTIONS option1 [option2 ...]]
                [OPTIONS option1 [option2 ...]]
                [FEATURES feature1 [feature2 ...]]
                [PRIVATE_FEATURES feature1 [feature2 ...]]
                [VERSION version]")
        return()
    endif()

    if(NOT DEFINED PARA_NAME)
        set(PARA_NAME ${PROJECT_NAME})
    endif()

    if(NOT DEFINED PARA_PLUGIN_TYPE)
        set(PARA_PLUGIN_TYPE App)
    endif()

    #更改输出目录到根目录
    if(ANDROID)
        SET(PLUGIN_DIR "libs/${ANDROID_ABI}")
    else()
        SET(PLUGIN_DIR "plugins/${PARA_PLUGIN_TYPE}/${PARA_NAME}")
    endif()
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/${PLUGIN_DIR}")
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${PLUGIN_DIR}")
    endif()

    #翻译资源
    set(QM_INSTALL_DIR ${PLUGIN_DIR}/translations)
    if(ANDROID)
        set(QM_INSTALL_DIR assets/plugins/translations)
    endif()
    GENERATED_QT_TRANSLATIONS(
        SOURCES ${PARA_SOURCE_FILES}
        OUT_QRC TRANSLATIONS_QRC_FILES
        QM_INSTALL_DIR ${QM_INSTALL_DIR})
    string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
    if(BUILD_TYPE STREQUAL "debug")
        list(APPEND PARA_RESOURCE_FILES ${TRANSLATIONS_QRC_FILES})
    endif()
    add_library(${PARA_NAME} ${PARA_SOURCE_FILES} ${PARA_RESOURCE_FILES})

    set_target_properties(${PARA_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PLUGIN_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PLUGIN_DIR}"
        )

    #安装
    INSTALL(TARGETS ${PARA_NAME}
            RUNTIME DESTINATION "${PLUGIN_DIR}"
                 COMPONENT Runtime
            LIBRARY DESTINATION "${PLUGIN_DIR}"
                 COMPONENT Runtime
            ARCHIVE DESTINATION "${PLUGIN_DIR}"
                 COMPONENT Runtime
            )

    if(DEFINED PARA_VERSION)
        set_target_properties(${PARA_NAME} PROPERTIES
            VERSION ${PARA_VERSION})
    endif()

    if(DEFINED PARA_LIBS)
        target_link_libraries(${PARA_NAME} PUBLIC ${PARA_LIBS})
    endif()

    if(DEFINED PARA_PRIVATE_LIBS)
        target_link_libraries(${PARA_NAME} PRIVATE ${PARA_PRIVATE_LIBS})
    endif()

    if(DEFINED PARA_DEFINITIONS)
        target_compile_definitions(${PARA_NAME} PUBLIC ${PARA_DEFINITIONS})
    endif()

    if(DEFINED PARA_PRIVATE_DEFINITIONS)
        target_compile_definitions(${PARA_NAME} PRIVATE ${PARA_PRIVATE_DEFINITIONS})
    endif()

    if(DEFINED PARA_INCLUDE_DIRS)
        target_include_directories(${PARA_NAME} PRIVATE ${PARA_INCLUDE_DIRS})
    endif()

    if(DEFINED PARA_OPTIONS)
        target_compile_options(${PARA_NAME} PUBLIC ${PARA_OPTIONS})
    endif()

    if(DEFINED PARA_PRIVATE_OPTIONS)
        target_compile_options(${PARA_NAME} PRIVATE ${PARA_PRIVATE_OPTIONS})
    endif()

    if(DEFINED PARA_FEATURES)
        target_compile_features(${PARA_NAME} PUBLIC ${PARA_FEATURES})
    endif()

    if(DEFINED PARA_PRIVATE_FEATURES)
        target_compile_features(${PARA_NAME} PRIVATE ${PARA_PRIVATE_FEATURES})
    endif()

    # 分发
#    IF(WIN32 AND BUILD_SHARED_LIBS)
#        IF(MINGW)
#            # windeployqt 分发时，是根据是否 strip 来判断是否是 DEBUG 版本,而用mingw编译时,qt没有自动 strip
#            add_custom_command(TARGET ${PARA_NAME} POST_BUILD
#                COMMAND strip "$<TARGET_FILE:${PARA_NAME}>"
#                )
#        ENDIF()

#        #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
#        add_custom_command(TARGET ${PARA_NAME} POST_BUILD
#            COMMAND "${QT_INSTALL_DIR}/bin/windeployqt"
#            --compiler-runtime
#            --verbose 7
#            "$<TARGET_FILE:${PARA_NAME}>"
#            )
#    ENDIF()

    IF(BUILD_SHARED_LIBS)
        #windows下动态库
        target_compile_definitions(${PARA_NAME} PRIVATE -DBUILD_SHARED_LIBS)
    ENDIF()

    #链接库
    target_link_libraries(${PARA_NAME} RabbitIm ${RABBITIM_LIBS})
endfunction()
