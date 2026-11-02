cmake_minimum_required(VERSION 3.16)
project(LogGuard VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(logguard_core src/LogParser.cpp)
target_include_directories(logguard_core PUBLIC include)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(logguard_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(test_parser tests/test_parser.cpp)
    target_link_libraries(test_parser PRIVATE logguard_core)
    add_test(NAME parser_unit_tests COMMAND test_parser)
endif()
