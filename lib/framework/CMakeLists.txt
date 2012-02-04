cmake_minimum_required(VERSION 2.6)

if(PANDORA)
  add_definitions(-DGLES1)
  add_definitions(-DPANDORA)
  #find_package(X11)
else(PANDORA)
  find_package(OpenGL)
endif(PANDORA)

find_package (SDL)
find_package (SDL_image)

#set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_FLAGS "-Wall")

file (GLOB_RECURSE SRCS src/*.cpp)

add_library(framework ${SRCS})
#add_executable(pandora-opengl-test src/main src/dlWindow src/vec2d src/quad src/image src/screen src/sprite src/transformation src/engine src/teststate src/world src/testentity)

if(PANDORA)
  set(LIBS ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} GLES_CM EGL X11)
else(PANDORA)
  set(LIBS ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${OPENGL_LIBRARY})
endif(PANDORA)

target_link_libraries(framework ${LIBS})

