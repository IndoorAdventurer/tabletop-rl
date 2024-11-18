cmake_minimum_required(VERSION 3.22)
project(Tabletop-RL VERSION 0.1)

# Compiler settings:
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "GNU compiler detected. Adding extra flags.")
    add_compile_options(-Wall)
else()
    message(STATUS "Unknown compiler. You might want to tweak CMakeLists.txt")
endif()

# Enabling Tests using Google Test:
enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# The cpp code base (makes library "cpp_archive"):
add_subdirectory(src/cpp/)

# Tests:
add_subdirectory(tests/cpp/)