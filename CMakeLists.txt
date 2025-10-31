cmake_minimum_required(VERSION 3.12)
project(mp2_lab3_arithmetic CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Build GoogleTest from provided single translation unit
add_library(gtest STATIC
    gtest/gtest-all.cc
)
target_include_directories(gtest PUBLIC ${CMAKE_SOURCE_DIR}/gtest)
if(UNIX)
    target_link_libraries(gtest PUBLIC pthread)
endif()

# Stack tests
add_executable(test_stack
    test/test_main.cpp
    test/test_stack.cpp
)
target_link_libraries(test_stack PRIVATE gtest)
target_compile_features(test_stack PRIVATE cxx_std_17)

# Arithmetic tests  
add_executable(test_arithmetic
    test/test_main.cpp
    test/test_arithmetic.cpp
)
target_link_libraries(test_arithmetic PRIVATE gtest)
target_compile_features(test_arithmetic PRIVATE cxx_std_17)

enable_testing()
add_test(NAME stack_tests COMMAND test_stack)
add_test(NAME arithmetic_tests COMMAND test_arithmetic)
