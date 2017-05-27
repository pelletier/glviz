#include <iostream>

#include "gl.h"
#include "glm.h"
#include "shader.h"
#include "Obj.h"
#include "Xyz.h"
#include "Renderer.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "glviz", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create visualization window." << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize glew." << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSetKeyCallback(window, key_callback);

    std::cout << "Loading shaders" << std::endl;
    shader::Shader simple_shader("simple.vert", "simple.frag");

    Obj obj("/Users/tpelletier/code/pelletier/glviz/models/cube/", "cube.Obj", simple_shader);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode

    glm::mat4 camera_transform_mat;
    camera_transform_mat = glm::rotate(camera_transform_mat, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    camera_transform_mat = glm::scale(camera_transform_mat, glm::vec3(0.5f, 0.5f, 0.5f));

    std::cout << glm::to_string(camera_transform_mat) << std::endl;

    Xyz xyz;

    Renderer renderer;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.render(obj, camera_transform_mat);
        renderer.render(xyz, camera_transform_mat);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
