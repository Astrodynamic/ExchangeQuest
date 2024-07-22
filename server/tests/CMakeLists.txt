cmake_minimum_required(VERSION 3.5)
project(TEST VERSION 1.0 LANGUAGES CXX)

#=============================================================== Setup CMake
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(
  ${PROJECT_SOURCE_DIR}/../../common/include
  ${PROJECT_SOURCE_DIR}/../src
)

set(HEADERS
  ../../common/include/common.h
  ../../common/include/order.h
  ../src/acount.h
  ../src/exchange.h
)

set(SOURCES
  ../src/acount.cc
  ../src/exchange.cc
)

enable_testing()

add_executable(${PROJECT_NAME}
  ${HEADERS}
  ${SOURCES}
  test.cc
)

target_link_libraries(${PROJECT_NAME}
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})