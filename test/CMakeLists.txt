CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include_directories(
  ../src
)


add_executable(test_vec2d test_vec2d.cpp ../src/util/vec2d.cpp ../src/util/transformation.cpp ../src/util/segment.cpp)
