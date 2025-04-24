cmake_minimum_required(VERSION 3.21)
project(toptw_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PROJECT_SOURCE_DIR "./src")

# 打印信息
message(STATUS "src This is BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "src This is SOURCE dir " ${PROJECT_SOURCE_DIR})

#set (CMAKE_C_COMPILER "/usr/local/gcc/bin/gcc")
#set (CMAKE_CXX_COMPILER "/usr/local/gcc/bin/g++")
# add_compile_options(-O2)
add_compile_options(-Wall)

# target: runToptw
add_subdirectory("${PROJECT_SOURCE_DIR}/toptw")
add_executable(runToptw "${PROJECT_SOURCE_DIR}/mainToptw.cpp")
target_link_libraries(runToptw PUBLIC toptw)
target_include_directories(runToptw PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/toptw")

