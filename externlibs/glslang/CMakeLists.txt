cmake_minimum_required(VERSION 3.25)

# build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

add_subdirectory(glslang)
add_subdirectory(OGLCompilersDLL)
add_subdirectory(SPIRV)
