
#生成目标
IF(PLUGIN_SOURCES)
    IF(OPTION_RABBITIM_USE_STATIC)
        add_library(${PROJECT_NAME}
            ${PLUGIN_SOURCES}
            ${PLUGIN_UIS}
            )
    ELSE(OPTION_RABBITIM_USE_STATIC)
        #windows下动态库导出
        ADD_DEFINITIONS(-DRABBITIM_SHARED_LIBRARY)
        add_library(${PROJECT_NAME} SHARED
            ${PLUGIN_SOURCES}
            ${PLUGIN_UIS}
            )
    ENDIF(OPTION_RABBITIM_USE_STATIC)
    
    #链接库
    target_link_libraries(${PROJECT_NAME}
            ${RABBITIM_LIBS}
            RabbitIm
            )
ENDIF(PLUGIN_SOURCES)

#复制插件到 ${CMAKE_BINARY_DIR}/plugins/App/${PROJECT_NAME}
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/plugins/App/${PROJECT_NAME}"
        COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}" "${CMAKE_BINARY_DIR}/plugins/App/${PROJECT_NAME}/."
        )

INSTALL(TARGETS ${PROJECT_NAME}
            LIBRARY DESTINATION "plugins/App/${PROJECT_NAME}"
            ARCHIVE DESTINATION "plugins/App/${PROJECT_NAME}")
