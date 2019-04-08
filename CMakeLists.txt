cmake_minimum_required(VERSION 2.8)

project(hkk18_alien)

add_executable(hkk18_alien.out hkk18_alien.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries( hkk18_alien.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
