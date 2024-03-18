cmake_minimum_required(VERSION 3.15) # cmake版本

project(thread_pool_c) # 项目名称

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clangd
# set(CMAKE_CXX_STANDARD 11) # c++11标准
set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib) # 动静态库目录
set(EXE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin) # 可执行文件目录
set(INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include) # 头文件目录
# 静态库
set(LIB_THREAD_POOL thread_pool)
set(LIB_QUEUE queue)
# 可执行程序
set(EXE_TEST test)
# 下面的子目录
add_subdirectory(thread_pool)
add_subdirectory(queue)
add_subdirectory(tests)
