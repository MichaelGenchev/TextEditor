cmake_minimum_required(VERSION 3.10)

# Set the project name
project(RopeProject)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find all the source files in the src directory
file(GLOB SOURCES "src/*.cpp")

# Print the sources (for debugging)
message("Source files: ${SOURCES}")

# Add executable target
add_executable(rope_editor ${SOURCES})

# If you have any include directories, add them like this:
target_include_directories(rope_editor PRIVATE include)

# If you're using any libraries, link them like this:
# target_link_libraries(rope_editor PRIVATE SomeLibrary)