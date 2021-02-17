cmake_minimum_required(VERSION 3.17)
project(astar)

set(CMAKE_CXX_STANDARD 14)



add_executable(astar main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Included these two lines:

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(astar ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})