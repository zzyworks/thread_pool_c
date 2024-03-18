cmake_minimum_required(VERSION 3.15) # cmake版本

project(tests) # 项目名称

aux_source_directory(./ SRC)

include_directories(${INCLUDE_PATH})

link_directories(${LIB_PATH})
link_libraries(${LIB_QUEUE})
link_libraries(${LIB_THREAD_POOL})

set(EXECUTABLE_OUTPUT_PATH ${EXE_PATH})

add_compile_options(-pthread -g  -ggdb -O0 -Wall   -fstack-protector-all -march=native -fnon-call-exceptions)
add_executable(${EXE_TEST} ${SRC})
