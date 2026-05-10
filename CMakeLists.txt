cmake_minimum_required(VERSION 3.15)
project(CursedTomorrow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(FETCH_RAYLIB "Fetch raylib from GitHub" OFF)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

if(FETCH_RAYLIB)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(raylib)
    target_link_libraries(${PROJECT_NAME} raylib)
else()
    find_package(raylib REQUIRED)
    target_link_libraries(${PROJECT_NAME} raylib)
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} winmm)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} m pthread dl rt X11)
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(DIRECTORY assets DESTINATION bin)

enable_testing()
add_executable(unit_tests src/tests/test_main.cpp)
target_include_directories(unit_tests PRIVATE include)
add_test(NAME AllTests COMMAND unit_tests)
