# TCHANZ

TCHanz is an object oriented C++ project that simulates ants colonies using GTKmm as graphical user interface. This README provides instructions on how to install dependencies, compile, and run the project.

## Table of Contents
- [Prerequisites](#prerequisites)
- [Compiling and Running the Project](#compiling-the-project)


## Prerequisites

Before you begin, ensure you have the following installed:

- **CMake**: A cross-platform build system generator. Install it via your package manager.
- **g++**: The GNU C++ compiler.
- **GTKmm**: The C++ interface for the GTK GUI toolkit.

### On Ubuntu/Debian

You can install the necessary packages with the following command:

```bash
sudo apt update
sudo apt install build-essential cmake libgtkmm-3.0-dev pkg-config
```


## Compiling and Running the Project

To compile the project, run the following command in the `build` directory:

```bash
./compile.sh
```

This script will clean the build directory, configure the project with CMake, and compile the source files.

