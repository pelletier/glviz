cmake_minimum_required(VERSION 3.7.1)

project(glviz)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(glviz
        src/main.cpp
        src/Shader.cpp
        src/Shader.h
        src/Obj.cpp
        src/Obj.h
        src/Xyz.cpp
        src/Xyz.h
        src/Drawable.h
        src/Renderer.cpp
        src/Renderer.h
        src/Mouse.h
        src/Mouse.cpp
        src/Font.cpp
        src/Font.h
        src/Freetype.h
        src/Freetype.cpp
        src/Text.h
        src/vendor/tiny_obj_loader.h
        src/vendor/stb_image.h src/Camera.cpp src/Camera.h)

set_property(TARGET glviz PROPERTY CXX_STANDARD 14)
set_property(TARGET glviz PROPERTY CXX_STANDARD_REQUIRED ON)

if(MSVC)
else()
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Werror")
endif()

if(WIN32)
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/win64/cmake/)
	set(GLFW_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/win64/glfw-3.2.1)
	set(GLEW_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/win64/glew-2.0.0)
	set(GLM_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/win64/glm-0.9.8.4)
	
	add_subdirectory(win64/freetype-2.8)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/win64/freetype-2.8/include)
	target_link_libraries(glviz freetype)

	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/win64/glew-2.0.0/include)
	target_link_libraries(glviz ${CMAKE_CURRENT_SOURCE_DIR}/win64/glew-2.0.0/lib/Release/x64/glew32s.lib)
else()
	find_package(Freetype REQUIRED)
	include_directories(${FREETYPE_INCLUDE_DIRS})
	target_link_libraries(glviz ${FREETYPE_LIBRARIES})

	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIR})
	target_link_libraries(glviz ${GLEW_LIBRARY})
endif()

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)

include_directories(${GLFW_INCLUDE_DIR})
target_link_libraries(glviz ${GLFW_LIBRARIES})

include_directories(${GLM_INCLUDE_DIR})

include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(glviz ${OPENGL_gl_LIBRARY})