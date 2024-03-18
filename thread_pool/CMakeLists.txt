cmake_minimum_required(VERSION 3.15) # cmake版本

project(thread_pool_c) # 项目名称

include_directories(${INCLUDE_PATH}) # 头文件搜索目录

aux_source_directory(./ SRC)

link_directories(${LIB_PATH})
link_libraries(${LIB_QUEUE})

set(LIBRARY_OUTPUT_PATH ${LIB_PATH}) # 可执行文件生成目录

add_library(${LIB_THREAD_POOL} STATIC ${SRC}) # 生成可执行文件
