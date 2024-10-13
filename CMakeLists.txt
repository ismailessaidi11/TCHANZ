# CMakeLists.txt

# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(tchanz)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compile options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

# Find gtkmm-3.0 package
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

# Include directories for GTK
include_directories(${GTKMM_INCLUDE_DIRS} include)

# Define source files by collecting all .c files in the src directory
file(GLOB_RECURSE SOURCES "src/*.cc")

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES})

# Add a custom command for generating dependencies
add_custom_target(depend
    COMMAND ${CMAKE_COMMAND} -E echo "*** MISE A JOUR DES DEPENDANCES ***"
)

# Alternatively, if you want to clear all generated files
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E echo "*** Cleaning up all build artifacts... ***"
    COMMAND ${CMAKE_COMMAND} -E remove -rf ${CMAKE_BINARY_DIR}/*
)

