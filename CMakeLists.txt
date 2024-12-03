cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(Snake_Game)

add_executable(${PROJECT_NAME} main.cpp)

find_package(raylib CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)