# CMake 最低版本号要求

cmake_minimum_required (VERSION 2.8)

# 项目信息

project (games)

# 指定生成目标

add_executable(games main.cpp src/g.cpp src/games/g2048.cpp src/games/gmines.cpp)
