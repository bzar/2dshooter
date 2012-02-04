cmake_minimum_required(VERSION 2.6)
project(2dshooter) 

if(PANDORA)
  add_definitions(-DPANDORA)
endif(PANDORA)

#set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_FLAGS "-Wall")

include_directories(lib/framework/include)
add_subdirectory (lib/framework)

file (GLOB SRCS src/*.cpp)

add_executable(2dshooter ${SRCS})
target_link_libraries(2dshooter framework)


install(TARGETS 2dshooter DESTINATION .)
install(FILES img/icon.png DESTINATION img)