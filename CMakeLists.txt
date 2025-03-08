cmake_minimum_required(VERSION 3.15)
project(SSRS LANGUAGES CXX)

# 设置C++17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含Conan生成的工具链配置
include("${CMAKE_CURRENT_SOURCE_DIR}/conan_toolchain.cmake")

# 添加可执行文件
add_executable(ssrs main.cpp)

# 查找并链接cJSON库
find_package(cjson REQUIRED)
target_link_libraries(ssrs PRIVATE cjson::cjson)