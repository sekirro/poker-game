cmake_minimum_required(VERSION 3.6)

set(APP_NAME PokerGame)
project(${APP_NAME})

# Cocos2d-x路径（需要根据实际安装路径修改）
set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源文件列表
set(GAME_SOURCE
    Classes/AppDelegate.cpp
    Classes/HelloWorldScene.cpp
    Classes/utils/CardDefines.h
    Classes/models/CardModel.cpp
    Classes/models/CardModel.h
    Classes/models/GameModel.cpp
    Classes/models/GameModel.h
    Classes/models/UndoModel.cpp
    Classes/models/UndoModel.h
    Classes/configs/models/LevelConfig.cpp
    Classes/configs/models/LevelConfig.h
    Classes/configs/models/CardResConfig.cpp
    Classes/configs/models/CardResConfig.h
    Classes/configs/loaders/LevelConfigLoader.cpp
    Classes/configs/loaders/LevelConfigLoader.h
    Classes/services/GameModelFromLevelGenerator.cpp
    Classes/services/GameModelFromLevelGenerator.h
    Classes/managers/UndoManager.cpp
    Classes/managers/UndoManager.h
    Classes/views/CardView.cpp
    Classes/views/CardView.h
    Classes/views/GameView.cpp
    Classes/views/GameView.h
    Classes/controllers/GameController.cpp
    Classes/controllers/GameController.h
)

# 头文件目录
set(GAME_HEADERS
    Classes
)

# Windows平台特定配置
if(WIN32)
    list(APPEND GAME_SOURCE proj.win32/main.cpp)
    list(APPEND GAME_HEADERS proj.win32)
endif()

# 包含Cocos2d-x
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${CMAKE_CURRENT_BINARY_DIR}/cocos2d)

# 添加可执行文件
if(WIN32)
    add_executable(${APP_NAME} WIN32 ${GAME_SOURCE})
else()
    add_executable(${APP_NAME} ${GAME_SOURCE})
endif()

# 包含目录
target_include_directories(${APP_NAME} PRIVATE ${GAME_HEADERS})

# 链接Cocos2d-x库
target_link_libraries(${APP_NAME} cocos2d)

# 设置资源目录
set(APP_RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
if(WIN32)
    set_target_properties(${APP_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()

# 拷贝资源文件到构建目录
add_custom_command(TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${APP_RES_DIR} $<TARGET_FILE_DIR:${APP_NAME}>/Resources
) 