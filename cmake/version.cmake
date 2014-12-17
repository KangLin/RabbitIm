#TODO:发行版本时，需要修改下列值  
SET(MAJOR_VERSION_NUMBER 0)      #主版本
SET(MINOR_VERSION_NUMBER 1)       #次版本
SET(REVISION_VERSION_NUMBER 0)  #修订号

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Update/Version.h.template.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/Version.h
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Update/Update.xml.template.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/Update/Update_${RABBITIM_SYSTEM}.xml
)
