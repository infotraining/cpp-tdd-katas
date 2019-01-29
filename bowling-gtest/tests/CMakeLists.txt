set(PROJECT_TESTS ${PROJECT_ID}_tests)
message(STATUS "PROJECT_TESTS is: " ${PROJECT_TESTS})

project(${PROJECT_TESTS})

###############
# Unit tests
find_package(GTest MODULE REQUIRED)

file(GLOB TEST_SOURCES *_tests.cpp *_test.cpp)
add_executable(${PROJECT_TESTS} ${TEST_SOURCES})
target_link_libraries(${PROJECT_TESTS} PRIVATE ${PROJECT_LIB} GTest::GTest GTest::Main)
target_compile_features(${PROJECT_TESTS} PUBLIC cxx_std_17)

enable_testing()
add_test(AllTestsInMain ${PROJECT_TESTS})
