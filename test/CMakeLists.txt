cmake_minimum_required(VERSION 3.14)

project(test)

find_package(libxml2 REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_executable(test test.cpp)

target_include_directories(test PRIVATE ${LIBXML2_INCLUDE_DIR})
target_link_libraries(test PRIVATE libcpp-html-to-md ${LIBXML2_LIBRARIES} GTest::gtest GTest::gtest_main)
