cmake_minimum_required(VERSION 3.15) # cmake版本

project(queue) # 项目名称

include_directories(${INCLUDE_PATH}) # 头文件搜索目录

aux_source_directory(./ SRC)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH}) # 可执行文件生成目录

add_library(${LIB_QUEUE} STATIC ${SRC}) # 生成可执行文件
