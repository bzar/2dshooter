CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(2dshooter)

add_subdirectory(lib)

include_directories(
  ${kazmath_SOURCE_DIR}/src
  ${glhck_SOURCE_DIR}/include
  ${GLFW_SOURCE_DIR}/include
  ${engineworks_SOURCE_DIR}/include
  ${qmlon_SOURCE_DIR}/include
)

file(GLOB SOURCES src/*.cpp src/util/*.cpp)
add_definitions(-DGLHCK_KAZMATH_FLOAT -DUSE_SINGLE_PRECISION)
list(APPEND CMAKE_CXX_FLAGS -std=c++0x)
add_executable(2dshooter ${SOURCES})

target_link_libraries(2dshooter engineworks glfw glhck qmlon ${GLFW_LIBRARIES})

file(COPY puppets DESTINATION .)
file(COPY skeletons DESTINATION .)
file(COPY spritesheets DESTINATION .)
file(COPY img DESTINATION .)

add_subdirectory(test)