cmake_minimum_required(VERSION 3.7)
project(music-wave)

# cmake 配置文件所在路径
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/" )

# 程序图标
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

# 会在上面配置的cmake路径中查找配置文件，FindSDL2.cmake
find_package(SDL2 REQUIRED)

# SDL2_INCLUDE_DIRS，SDL2_LIBRARIES 等 变量是根据 FindSDL2.cmake 的配置文件生成的
include_directories(. ${SDL2_INCLUDE_DIRS})  

# 生成可执行文件
add_executable(music-wave ${CMAKE_CURRENT_SOURCE_DIR}/music-wave.c ${APP_ICON_RESOURCE_WINDOWS})

# 链接 SDL2
target_link_libraries(music-wave ${SDL2_LIBRARIES} -liconv -mwindows)

