cmake_minimum_required(VERSION 3.26)
project(search_engine)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(nlohmann_json)

add_executable(search_engine_executable main.cpp
        include/ConverterJSON.h
        src/InvertedIndex.cpp
        src/SearchServer.cpp
        src/ConverterJson.cpp)
target_link_libraries(search_engine_executable PRIVATE nlohmann_json::nlohmann_json ${CMAKE_EXE_LINKER_FLAGS}
        )

enable_testing()
include_directories(search_engine_executable PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_subdirectory(Google_tests/lib)
add_executable(
        search_engine_tests
        Google_tests/InverterIndexTest.cpp
        Google_tests/SearchServerTest.cpp
        include/InvertedIndex.h
        src/ConverterJson.cpp
        src/InvertedIndex.cpp
        include/SearchServer.h
        src/SearchServer.cpp
)

set_target_properties(search_engine_tests PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(search_engine_tests gtest gtest_main)