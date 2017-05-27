cmake_minimum_required(VERSION 3.7.1)

project(glviz)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(glviz src/main.cpp src/Shader.cpp src/Shader.h src/tiny_obj_loader.h src/Obj.cpp src/Obj.h src/Xyz.cpp src/Xyz.h src/Drawable.h src/Renderer.cpp src/Renderer.h src/Mouse.h src/Mouse.cpp)

set_property(TARGET glviz PROPERTY CXX_STANDARD 14)
set_property(TARGET glviz PROPERTY CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Werror")

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)

target_link_libraries(glviz glfw)

include_directories(${GLM_INCLUDE_DIR})

include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(glviz ${OPENGL_gl_LIBRARY})

include_directories(${GLEW_INCLUDE_DIR})
target_link_libraries(glviz ${GLEW_LIBRARY})
