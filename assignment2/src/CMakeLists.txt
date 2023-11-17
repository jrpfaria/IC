cmake_minimum_required(VERSION 3.16)
project(assignment2)

SET (CMAKE_BUILD_TYPE "Release")
#SET (CMAKE_BUILD_TYPE "Debug")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17")
SET (CMAKE_CXX_FLAGS_RELEASE "-O3")
#SET (CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=address")

SET (BASE_DIR ${CMAKE_SOURCE_DIR} )
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/../bin)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable (ppm_effects ppm_effects.cpp)
target_link_libraries (ppm_effects ${OpenCV_LIBS})

add_executable (golomb golomb.cpp)
target_link_libraries (golomb)

add_executable (audio_encoder audio_encoder.cpp)
target_link_libraries (audio_encoder sndfile)
