cmake_minimum_required(VERSION 2.8.8)

project(imgtools)

add_subdirectory("external/stb")
add_subdirectory("external/tinyexr")

include_directories(${STB_PATH})
include_directories(${TINYEXR_PATH})
include_directories("include")

add_executable(resize
	src/resize.cpp
	src/image.cpp)

add_executable(convert
	src/convert.cpp
	src/image.cpp)

add_executable(mean_image_brightness
	src/mean_image_brightness.cpp
	src/image.cpp)

add_executable(white_balance
	src/white_balance.cpp
	src/image.cpp)

add_dependencies(resize stb tinyexr)
add_dependencies(convert stb tinyexr)
add_dependencies(mean_image_brightness stb tinyexr)
add_dependencies(white_balance stb tinyexr)
