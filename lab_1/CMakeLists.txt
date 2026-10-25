cmake_minimum_required(VERSION 3.14)

project(Restaurant LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(restaurant
    src/main.cpp
)

target_include_directories(restaurant PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include(CTest)

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
    )

    FetchContent_MakeAvailable(googletest)

    add_executable(restaurant_tests
        tests/test_dish.cpp
        tests/test_order.cpp
        tests/test_kitchen.cpp
    )

    target_include_directories(restaurant_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(restaurant_tests PRIVATE
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(restaurant_tests)
endif()
