cmake_minimum_required(VERSION 3.21)

project(CommonLib VERSION 0.0.1 LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(${PROJECT_NAME}
    INTERFACE
    cxx_std_20
)
