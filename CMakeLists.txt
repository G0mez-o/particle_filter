cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(particle_fileter)

set(CMAKE_BUILD_TYPE release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -s")

add_compile_options(-std=c++11)

find_package(OpenCV 3.3 REQUIRED)

include_directories(
  include
  ${OpenCV_INCLUDE_DIRS})

add_executable (particle particle_filter.cpp)
target_link_libraries (particle ${OpenCV_LIBS})
