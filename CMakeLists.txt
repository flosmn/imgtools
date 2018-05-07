cmake_minimum_required(VERSION 2.8.8)

project(imgtools)

add_subdirectory("external/stb")
add_subdirectory("external/tinyexr")

include_directories(${STB_PATH})
include_directories(${TINYEXR_PATH})

add_executable(imgtools
	src/imgtools.cpp)

add_executable(resize
	src/resize.cpp)

add_dependencies(imgtools stb tinyexr)
add_dependencies(resize stb tinyexr)
