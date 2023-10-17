# Set VCPKG_ROOT to your vcpkg installation directory or add the following to your cmake call:
# -DCMAKE_TOOLCHAIN=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake

cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")  
endif()
if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

project(cpp-tdd-katas LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}/bin")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}/bin")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}/lib")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}/lib")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}/lib")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}/lib")

enable_testing()

# SET(ENABLE_COVERAGE True)

if(NOT MSVC AND ENABLE_COVERAGE)
    # set compiler flags
    set(CMAKE_CXX_FLAGS "-O0 -coverage")

  # find required tools
  find_program(LCOV lcov REQUIRED)
  find_program(GENHTML genhtml REQUIRED)

  cmake_path(SET vcpkg_path "${CMAKE_SOURCE_DIR}/vcpkg" )
  cmake_path(SET vcpkg_installed_path "${CMAKE_SOURCE_DIR}/build/vcpkg_installed" )

  message(STATUS "Vcpkg path: \"${vcpkg_path}\"")
  message(STATUS "Vcpkg_installed path: \"${vcpkg_installed_path}\"")

  # add coverage target
  add_custom_target(coverage
    # gather data
    COMMAND ${LCOV} --directory . --capture --output-file coverage.info
    COMMAND ${LCOV} --remove coverage.info "/usr/*" "${vcpkg_path}/*" "${vcpkg_installed_path}/*"  -o coverage.info.filtered
    # generate report
    COMMAND ${GENHTML} --demangle-cpp -o coverage coverage.info.filtered 
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR} VERBATIM)

  message(STATUS "Test coverage is ON")
endif()

add_subdirectory(bowling-catch)
add_subdirectory(bowling-gtest)
add_subdirectory(recently-used-list-catch)
add_subdirectory(recently-used-list-gtest)
add_subdirectory(mars-rover-catch)