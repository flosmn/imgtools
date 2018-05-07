cmake_minimum_required(VERSION 2.8.8)

project(imgtools)

add_subdirectory("external/stb")
include_directories(${STB_PATH})

add_executable(imgtools
	src/imgtools.cpp)

add_dependencies(imgtools stb)
