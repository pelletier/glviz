#include <iostream>

#include "gl.h"
#include "glm.h"
#include "Shader.h"
#include "Obj.h"
#include "Xyz.h"
#include "Renderer.h"

static bool keys[1024];

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

static bool first_mouse_pos = true;
static GLfloat mouse_last_x = 0;
static GLfloat mouse_last_y = 0;
static GLfloat mouse_x = 0;
static GLfloat mouse_y = 0;
static const GLfloat mouse_sensitivity = 0.1f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    mouse_x = (GLfloat) xpos;
    mouse_y = (GLfloat) ypos;
}

static bool mouse_right_button_down = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        switch (action) {
            case GLFW_PRESS:
                mouse_right_button_down = true;
                first_mouse_pos = true;
                break;
            case GLFW_RELEASE:
                mouse_right_button_down = false;
                break;
            default:
                break;
        }
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
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    mouse_last_x = width / 2;
    mouse_last_y = height / 2;

    std::cout << "Loading shaders" << std::endl;
    shader::Shader simple_shader("simple.vert", "simple.frag");
    Xyz xyz;

    Obj obj("/Users/tpelletier/code/pelletier/glviz/models/cube/", "cube.Obj", simple_shader);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    Renderer renderer;

    GLfloat last_time = 0.0f;

    GLfloat camera_pitch = 0.0f;
    GLfloat camera_yaw = -90.0f;

    while (!glfwWindowShouldClose(window)) {
        GLfloat current_time = (GLfloat) glfwGetTime();
        GLfloat delta_time = current_time - last_time;

        glfwPollEvents();

        // Process camera-related events

        if (first_mouse_pos) {
            first_mouse_pos = false;
            mouse_last_y = mouse_y;
            mouse_last_x = mouse_x;
        }

        if (mouse_right_button_down) {
            GLfloat mouse_x_offset = (GLfloat) (mouse_x - mouse_last_x) * mouse_sensitivity;
            GLfloat mouse_y_offset =
                    (GLfloat) (mouse_last_y - mouse_y) * mouse_sensitivity; // reverse to enable Y inversion
            mouse_last_x = mouse_x;
            mouse_last_y = mouse_y;

            camera_pitch += mouse_y_offset;
            if (camera_pitch > 89.0f) {
                camera_pitch = 89.0f;
            } else if (camera_pitch < -89.0f) {
                camera_pitch = -89.0f;
            }

            camera_yaw += mouse_x_offset;

            std::cout << "mouse_x_offset= " << mouse_x_offset << " mouse_y_offset=" << mouse_y_offset << " pitch = " << camera_pitch << " yaw = " << camera_yaw << std::endl;
        }

        static const GLfloat camera_speed = delta_time * 0.05f;
        static glm::vec3 camera_pos(0.0f, 0.0f, 3.0f);


        GLfloat cos_pitch = glm::cos(glm::radians(camera_pitch));
        GLfloat sin_pitch = glm::sin(glm::radians(camera_pitch));
        GLfloat cos_yaw = glm::cos(glm::radians(camera_yaw));
        GLfloat sin_yaw = glm::sin(glm::radians(camera_yaw));

        std::cout << "PITCH=" << camera_pitch << " COS()=" << cos_pitch << " SIN()=" << sin_pitch << std::endl;
        std::cout << "YAW=" << camera_yaw << " COS()=" << cos_yaw << " SIN()=" << sin_yaw << std::endl;

        glm::vec3 camera_direction = glm::normalize(glm::vec3(cos_pitch * cos_yaw,
                                                              sin_pitch,
                                                              cos_pitch * sin_yaw));
        static glm::vec3 camera_up(0.0f, 1.0f, 0.0f);

        if (keys[GLFW_KEY_W]) {
            camera_pos += camera_speed * camera_direction;
        }
        if (keys[GLFW_KEY_S]) {
            camera_pos -= camera_speed * camera_direction;
        }
        if (keys[GLFW_KEY_A]) {
            camera_pos -= camera_speed * glm::normalize(glm::cross(camera_direction, camera_up));
        }
        if (keys[GLFW_KEY_D]) {
            camera_pos += camera_speed * glm::normalize(glm::cross(camera_direction, camera_up));
        }

        // Compute camera
        std::cout << "direction = " << glm::to_string(camera_direction) << std::endl;
        glm::vec3 camera_target(camera_pos + camera_direction);
        glm::mat4 view = projection * glm::lookAt(camera_pos, camera_target, camera_up);


        // Render scene!
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.render(obj, view);
        renderer.render(xyz, view);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
