#include <iostream>

#include "gl.h"
#include "glm.h"
#include "Freetype.h"
#include "Font.h"
#include "Shader.h"
#include "Obj.h"
#include "Xyz.h"
#include "Renderer.h"
#include "Mouse.h"
#include "Text.h"

static bool keys[1024];

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
  if (action == GLFW_PRESS) {
    keys[key] = true;
  } else if (action == GLFW_RELEASE) {
    keys[key] = false;
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

  GLFWwindow *window = glfwCreateWindow(800, 600, "glviz", nullptr, nullptr);
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
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwSetKeyCallback(window, key_callback);
  Mouse::bind_glfw_callbacks(window);
  Mouse &mouse = Mouse::global_instance();

  Freetype freetype;
  font::Font consolas_font(freetype, "/Users/tpelletier/code/pelletier/glviz/fonts/consolas.ttf");

  std::cout << "Loading shaders" << std::endl;
  shader::Shader simple_shader("simple.vert", "simple.frag");
  Xyz xyz;

  Obj obj("/Users/tpelletier/code/pelletier/glviz/models/cube/", "cube.Obj", simple_shader);

  glm::mat4 projection = glm::perspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 100.0f);
  Renderer renderer(width, height);
  renderer.wireframe_mode = true;

  GLfloat last_time = 0.0f;

  GLfloat camera_pitch = 0.0f;
  GLfloat camera_yaw = -90.0f;

  Text text = {"hello world", glm::vec2(200, 100), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), consolas_font};

  while (!glfwWindowShouldClose(window)) {
    GLfloat current_time = (GLfloat) glfwGetTime();
    GLfloat delta_time = current_time - last_time;

    glfwPollEvents();

    // Process camera-related events

    if (mouse.is_right_button_down()) {
      const glm::vec2 offset = mouse.get_offset();

      camera_pitch += offset.y;
      if (camera_pitch > 89.0f) {
        camera_pitch = 89.0f;
      } else if (camera_pitch < -89.0f) {
        camera_pitch = -89.0f;
      }

      camera_yaw += offset.x;
    }

    static const GLfloat camera_speed = delta_time * 0.05f;
    static glm::vec3 camera_pos(0.0f, 0.0f, 3.0f);

    GLfloat cos_pitch = glm::cos(glm::radians(camera_pitch));
    GLfloat sin_pitch = glm::sin(glm::radians(camera_pitch));
    GLfloat cos_yaw = glm::cos(glm::radians(camera_yaw));
    GLfloat sin_yaw = glm::sin(glm::radians(camera_yaw));

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
    glm::vec3 camera_target(camera_pos + camera_direction);
    glm::mat4 view = projection * glm::lookAt(camera_pos, camera_target, camera_up);

    // Render scene!
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.render(obj, view);
    renderer.render(xyz, view);
    renderer.render(text);

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}
