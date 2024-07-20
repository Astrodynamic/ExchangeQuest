project(${CMAKE_PROJECT_NAME}Client VERSION 0.0.1 LANGUAGES CXX)

find_package(Boost 1.66 COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

set(HEADERS
    src/client.h
)

set(SOURCES
    src/client.cc
)

add_executable(${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
    src/main.cc
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
    cxx_std_20
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Threads::Threads
    ${Boost_LIBRARIES}
    CommonLib
)