cmake_minimum_required(VERSION 3.10)

project(Parser LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -ftree-vectorize -pipe")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Og -Wall -Wextra -pipe")

add_library(${PROJECT_NAME})
add_library(OpenW::Parser ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
       source/config_parser.cpp
       include/config_parser.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
