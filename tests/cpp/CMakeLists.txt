# Collecting all test source files:
file(GLOB_RECURSE TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/**test*.cpp
)

# Making the testing executable:
add_executable(cpp_tests_all ${TEST_FILES})

# Linking to the cpp archive and google test:
target_link_libraries(cpp_tests_all cpp_archive GTest::gtest_main)

# Adding the cpp library for easy imports:
target_include_directories(cpp_tests_all PRIVATE ${PROJECT_SOURCE_DIR}/src/cpp)

# Google test stuff:
include(GoogleTest)

gtest_discover_tests(cpp_tests_all)

# ------------------------------------------:
# Making an extra executable for experiments:
add_executable(experiments experiments.cpp)

# Linking to the cpp archive:
target_link_libraries(experiments cpp_archive)

# Adding the cpp library for easy imports:
target_include_directories(experiments PRIVATE ${PROJECT_SOURCE_DIR}/src/cpp)