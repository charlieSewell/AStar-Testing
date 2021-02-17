cmake_minimum_required(VERSION 3.17)
project(astar)

set(CMAKE_CXX_STANDARD 14)

set(SANITIZER_OS "Darwin,Linux")
set(SANITIZER_FLAGS "-fsanitize=address,undefined,leak")

add_executable(astar main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Included these two lines:

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()




include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(astar ${OPENGL_LIBRARIES} GLUT::GLUT)