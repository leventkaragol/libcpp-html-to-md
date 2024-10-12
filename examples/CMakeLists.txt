cmake_minimum_required(VERSION 3.14)

project(examples)

add_executable(examples main.cpp)

find_package(libxml2 REQUIRED)

target_include_directories(examples PRIVATE ${LIBXML2_INCLUDE_DIR})
target_link_libraries(examples PRIVATE libcpp-html-to-md ${LIBXML2_LIBRARIES})
