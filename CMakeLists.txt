cmake_minimum_required(VERSION 3.16)
project(LogGuard VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(logguard_core
    src/LogParser.cpp
    src/LogAnalyzer.cpp
)
target_include_directories(logguard_core PUBLIC include)

add_executable(logguard src/main.cpp)
target_link_libraries(logguard PRIVATE logguard_core)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(logguard_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(logguard PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(CTest)
if(BUILD_TESTING)
    # The two small C++ test executables use standard-library assertions/checks.
    # This avoids a network dependency just to run the project tests.
    add_executable(test_parser tests/test_parser.cpp)
    target_link_libraries(test_parser PRIVATE logguard_core)
    add_test(NAME parser_unit_tests COMMAND test_parser)

    add_executable(test_analyzer tests/test_analyzer.cpp)
    target_link_libraries(test_analyzer PRIVATE logguard_core)
    add_test(NAME analyzer_unit_tests COMMAND test_analyzer)

    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_test(NAME python_component_tests
             COMMAND ${Python3_EXECUTABLE} -m pytest -q
                     ${CMAKE_CURRENT_SOURCE_DIR}/tests/component)
    set_tests_properties(python_component_tests PROPERTIES
        ENVIRONMENT "LOGGUARD_BIN=$<TARGET_FILE:logguard>")
endif()
