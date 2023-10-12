set(PROJECT_TESTS ${PROJECT_ID}_tests)
message(STATUS "PROJECT_TESTS is: " ${PROJECT_TESTS})

project(${PROJECT_TESTS})

###############
# Unit tests
find_package(Catch2 CONFIG REQUIRED)

file(GLOB TEST_SOURCES *_tests.cpp *_test.cpp)
add_executable(${PROJECT_TESTS} ${TEST_SOURCES})
target_link_libraries(${PROJECT_TESTS} PRIVATE ${PROJECT_LIB} Catch2::Catch2WithMain)

enable_testing()
add_test(AllTestsInMain ${PROJECT_TESTS})
