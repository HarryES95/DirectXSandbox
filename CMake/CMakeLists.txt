# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("DirectX")

# Include sub-projects.
add_subdirectory ("CMake")

add_executable("CMake.cpp", "CMake.h")