cmake_minimum_required(VERSION 3.0.0)
project(BUKAS VERSION 0.0.1)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

# Automatically gather all .cpp files in src and lib directories
file(GLOB_RECURSE SOURCES
    src/*.cpp
    lib/*.c
    assets/*.cpp   
)

# Create the executable
add_executable(BUKAS ${SOURCES})

# Include directories
target_include_directories(BUKAS PRIVATE lib src)

# Link libraries
target_link_libraries(BUKAS glfw OpenGL::GL)
