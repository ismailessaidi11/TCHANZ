#!/bin/bash

# Define the project name (ensure it matches the one in CMakeLists.txt)
PROJECT_NAME="tchanz"

echo "Compiling project..."
mkdir build
cd build || { echo "Failed to change directory to build."; exit 1; }

# Clean the build directory
make clean

# Configure the project
cmake .. 

# Compile the project
make

# Check if the executable was created
if [ ! -f "${PROJECT_NAME}" ]; then
    echo "Error: Executable ${PROJECT_NAME} not found."
    exit 1
fi

echo "Running ${PROJECT_NAME}..."
./"${PROJECT_NAME}"
