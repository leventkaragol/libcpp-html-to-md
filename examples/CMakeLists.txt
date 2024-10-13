cmake_minimum_required(VERSION 3.14)

project(examples)

add_executable(examples main.cpp)

find_package(libxml2 REQUIRED)

IF (WIN32)
    find_package(Iconv REQUIRED)
ENDIF ()

target_include_directories(examples PRIVATE ${LIBXML2_INCLUDE_DIR})

IF (WIN32)
    target_link_libraries(examples PRIVATE libcpp-html-to-md ${LIBXML2_LIBRARIES} Iconv::Iconv)
ELSE ()
    target_link_libraries(examples PRIVATE libcpp-html-to-md ${LIBXML2_LIBRARIES})
ENDIF ()