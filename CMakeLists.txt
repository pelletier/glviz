cmake_minimum_required(VERSION 3.7.1)

project(glviz)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(glviz main.cpp)

set_property(TARGET glviz PROPERTY CXX_STANDARD 14)
set_property(TARGET glviz PROPERTY CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Werror")

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(glviz glfw)

include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(glviz ${OPENGL_gl_LIBRARY})

include_directories(${GLEW_INCLUDE_DIR})
target_link_libraries(glviz ${GLEW_LIBRARY})
