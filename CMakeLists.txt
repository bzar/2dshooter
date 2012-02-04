cmake_minimum_required(VERSION 2.6)
project(pandora-opengl-test) 

if(PANDORA)
  add_definitions(-DPANDORA)
endif(PANDORA)

#set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_FLAGS "-Wall")

include_directories(lib/framework/include)
add_subdirectory (lib/framework)

file (GLOB SRCS src/*.cpp)

add_executable(pandora-opengl-test ${SRCS})
target_link_libraries(pandora-opengl-test framework)

