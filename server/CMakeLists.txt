project(${CMAKE_PROJECT_NAME}Server VERSION 0.0.1 LANGUAGES CXX)

set(HEADERS
)

set(SOURCES
)

add_executable(${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
    src/main.cc
)

add_dependencies(${PROJECT_NAME}
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
    cxx_std_20
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
)