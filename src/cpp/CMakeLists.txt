# Collecting all source files:
file(GLOB_RECURSE CPP_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/**.cpp
)

# Creating a static library out of it:
add_library(cpp_archive ${CPP_LIB_SOURCES})