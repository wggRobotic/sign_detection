cmake_minimum_required(VERSION 3.10)
project(Sign_detection)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(main src/main.cpp)

# Link the necessary libraries
target_link_libraries(main ${OpenCV_LIBS})

# Include directories
target_include_directories(main PUBLIC include/)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
