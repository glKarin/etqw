cmake_minimum_required(VERSION 3.25)

project(vk_glsl)

SET(GLSLANG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../externlibs/glslang)

add_subdirectory(${GLSLANG_PATH} externlibs/glslang)

set(CMAKE_CXX_STANDARD "11")

add_library(idtech_vkglsl STATIC vk_glsl.cpp)

target_link_libraries(idtech_vkglsl "glslang" "SPIRV")

target_include_directories(idtech_vkglsl PRIVATE ${GLSLANG_PATH})
