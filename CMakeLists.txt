cmake_minimum_required(VERSION 3.10)

project(sign_detector VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(main src/main.cpp)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
