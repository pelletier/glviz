#include <iostream>

#include "gl.h"
#include "glm.h"
#include "Shader.h"
#include "Obj.h"
#include "Xyz.h"
#include "Renderer.h"


static const GLfloat camera_speed = 0.05f;
static glm::vec3 camera_pos(0.0f, 0.0f, 3.0f);
static glm::vec3 camera_direction(0.0f, 0.0f, -1.0f);
static glm::vec3 camera_up(0.0f, 1.0f, 0.0f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (key == GLFW_KEY_W) {
        camera_pos += camera_speed * camera_direction;
    } else if (key == GLFW_KEY_S) {
        camera_pos -= camera_speed * camera_direction;
    } else if (key == GLFW_KEY_A) {
        camera_pos -= camera_speed * glm::normalize(glm::cross(camera_direction, camera_up));
    } else if (key == GLFW_KEY_D) {
        camera_pos += camera_speed * glm::normalize(glm::cross(camera_direction, camera_up));
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
    glEnable(GL_DEPTH_TEST);

    glfwSetKeyCallback(window, key_callback);

    std::cout << "Loading shaders" << std::endl;
    shader::Shader simple_shader("simple.vert", "simple.frag");
    Xyz xyz;

    Obj obj("/Users/tpelletier/code/pelletier/glviz/models/cube/", "cube.Obj", simple_shader);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    Renderer renderer;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

//        const GLfloat time = (const GLfloat) glfwGetTime();

        glm::vec3 camera_target(camera_pos + camera_direction);
        glm::mat4 view = projection * glm::lookAt(camera_pos, camera_target, camera_up);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.render(obj, view);
        renderer.render(xyz, view);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
