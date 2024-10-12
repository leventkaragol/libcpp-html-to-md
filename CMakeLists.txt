cmake_minimum_required(VERSION 3.14)

project(libcpp-html-to-md)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(libcpp-html-to-md INTERFACE)

target_include_directories(libcpp-html-to-md INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

