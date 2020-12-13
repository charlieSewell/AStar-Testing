cmake_minimum_required(VERSION 3.17)
project(untitled1)

set(CMAKE_CXX_STANDARD 14)



add_executable(untitled1 main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Included these two lines:

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(untitled1 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})