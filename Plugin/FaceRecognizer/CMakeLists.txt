cmake_minimum_required(VERSION 2.8)

project(FaceRecognizer)

FIND_PACKAGE(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

IF(MSVC)
    SET(CMAKE_CXX_FLAGS "/MP") 
ENDIF(MSVC)

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

#判断 qt 版本
SET(QT_INSTALL_DIR "${Qt5_DIR}/../../..")
#需要的QT组件
SET(QT_COMPONENTS Core Gui Widgets)
FOREACH(_COMPONENT ${QT_COMPONENTS})
    FIND_PACKAGE(Qt5${_COMPONENT} REQUIRED)
    SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5${_COMPONENT}_LIBRARIES})
ENDFOREACH()

SET(SOURCE_FILES 
    main.cpp
    CapThread.cpp CapThread.h
    DetectFaces.cpp
    DetectFaces.h
    FrmFaceRecognizer.cpp
    FrmFaceRecognizer.h)

SET(SOURCE_UI_FILES FrmFaceRecognizer.ui)

message("Opencv_LIBS:${OpenCV_LIBS}")
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${SOURCE_UI_FILES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${QT_LIBRARIES})
