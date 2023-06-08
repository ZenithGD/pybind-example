# This is the main CMake build file
cmake_minimum_required (VERSION 3.16)
project(pybind-example LANGUAGES CXX VERSION 0.1)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ version selection")  # or 11, 14, 17, 20
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # optional, ensure standard is supported
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
SET(CMAKE_INSTALL_PREFIX dist)

# Add external dependencies
add_subdirectory(ext)

# Add the source code
add_subdirectory(src)
