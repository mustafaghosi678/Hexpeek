cmake_minimum_required(VERSION 3.16)
project(hexpeek CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra)

add_library(hexpeek_lib
    src/hex_formatter.cpp
    src/file_reader.cpp
    src/searcher.cpp
)
target_include_directories(hexpeek_lib PUBLIC src)

add_executable(hexpeek src/main.cpp)
target_link_libraries(hexpeek PRIVATE hexpeek_lib)

enable_testing()
add_executable(hexpeek_tests tests/test_hexpeek.cpp)
target_link_libraries(hexpeek_tests PRIVATE hexpeek_lib)
add_test(NAME hexpeek_tests
    COMMAND hexpeek_tests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
