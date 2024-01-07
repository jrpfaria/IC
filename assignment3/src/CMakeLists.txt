cmake_minimum_required(VERSION 3.16)
project(assignment3)

SET (CMAKE_BUILD_TYPE "Release")
#SET (CMAKE_BUILD_TYPE "Debug")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17")
SET (CMAKE_CXX_FLAGS_RELEASE "-O3")
#SET (CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=address")

SET (BASE_DIR ${CMAKE_SOURCE_DIR} )
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/../bin)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable (reader reader.cpp)
target_link_libraries (reader ${OpenCV_LIBS})

add_executable (video_encoder video_encoder.cpp)
target_link_libraries (video_encoder ${OpenCV_LIBS})

add_executable (video_decoder video_decoder.cpp)
target_link_libraries (video_decoder ${OpenCV_LIBS})
