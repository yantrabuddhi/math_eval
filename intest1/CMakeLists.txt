cmake_minimum_required(VERSION 3.30)
project(intest1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the paths to Raylib (adjust these to your actual paths)
set(RAYLIB_INCLUDE_DIR C:/raylib/raylib/cmake-build-debug/raylib/include)
set(RAYLIB_LIB_DIR C:/raylib/raylib/cmake-build-debug/raylib) # Or win32

# Find Raylib (using the variables we set above)
include_directories(${RAYLIB_INCLUDE_DIR} "../mpl1")
link_directories(${RAYLIB_LIB_DIR} "../mpl1/cmake-build-debug")

# Define your executable
add_executable(intest1 graph_sqrt_function.cpp)
# Link Raylib to your executable.  Important to specify the correct library name.
target_link_libraries(intest1 raylib winmm
        defmath
        meval)  # Or raylib.lib if needed

# Set output directory for the executable (optional)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable CLion's code inspection (optional)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

