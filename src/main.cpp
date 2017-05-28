#include <iostream>
#include <sstream>

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
#include "Camera.h"

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
  font::Font consolas_font(freetype, "/Users/tpelletier/code/pelletier/glviz/fonts/consolas.ttf", 20);

  std::cout << "Loading shaders" << std::endl;
  shader::Shader simple_shader("simple.vert", "simple.frag");
  Xyz xyz;

  Obj obj("/Users/tpelletier/code/pelletier/glviz/models/teapot/", "teapot.obj", simple_shader);

  glm::mat4 projection = glm::perspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 10000.0f);
  Renderer renderer(width, height);
  renderer.wireframe_mode = false;

  GLfloat last_time = 0.0f;
  camera::Camera camera;
  camera.mode = camera::CENTERED;

  while (!glfwWindowShouldClose(window)) {
    GLfloat current_time = (GLfloat) glfwGetTime();
    GLfloat delta_time = current_time - last_time;
    last_time = current_time;

    glfwPollEvents();

    // Process camera-related events

    glm::vec3 camera_target;

    if (keys[GLFW_KEY_SPACE]) {
      if (camera.mode == camera::FREE_FLIGHT) {
        camera.mode = camera::CENTERED;
        camera.offset_speed(camera.base_speed() * 9.0f);
      } else if (camera.mode == camera::CENTERED) {
        camera.mode = camera::FREE_FLIGHT;
        camera.offset_speed(camera.base_speed() * -0.9f);
      }
      keys[GLFW_KEY_SPACE] = false; // XXX: not clean, but good enough for now.
    }

    if (camera.mode == camera::FREE_FLIGHT) {
      camera.offset_speed(-mouse.get_scroll_offset().y);
      const GLfloat camera_speed = delta_time * camera.base_speed() * 0.3f;

      if (mouse.is_right_button_down()) {
        const glm::vec2 offset = mouse.get_offset();
        camera.offset_pitch(offset.y);
        camera.offset_yaw(offset.x);
      }
      camera.update_direction_from_angles();

      if (keys[GLFW_KEY_W]) {
        camera.pos += camera_speed * camera.direction;
      }
      if (keys[GLFW_KEY_S]) {
        camera.pos -= camera_speed * camera.direction;
      }
      if (keys[GLFW_KEY_A]) {
        camera.pos -= camera_speed * glm::normalize(glm::cross(camera.direction, camera.up));
      }
      if (keys[GLFW_KEY_D]) {
        camera.pos += camera_speed * glm::normalize(glm::cross(camera.direction, camera.up));
      }
      camera_target = camera.pos + camera.direction;
    } else if (camera.mode == camera::CENTERED) {

      if (keys[GLFW_KEY_W]) {
        camera.offset_speed(0.5);
      }
      if (keys[GLFW_KEY_S]) {
        camera.offset_speed(-0.5);
      }

      const GLfloat camera_speed = delta_time * camera.base_speed();
      const glm::vec3 center(0, 0, 0);
      const glm::vec3 direction = glm::normalize(center - camera.pos);

      if (mouse.is_right_button_down()) {
        const glm::vec2 offset = mouse.get_offset();
        const GLfloat theta = glm::radians(offset.x * camera_speed);
        const GLfloat dist = glm::distance(center, camera.pos);

        const glm::vec3 right = glm::normalize(glm::cross(direction, camera.up));
        camera.pos += direction * (dist - dist * glm::cos(theta)) - right * dist * glm::sin(theta);

        const GLfloat gamma = glm::radians(offset.y * camera_speed);

        const glm::vec3 up = glm::normalize(glm::cross(right, direction));
        camera.pos += direction * (dist - dist * glm::cos(gamma)) - up * dist * glm::sin(gamma);
      }

      camera.pos += mouse.get_scroll_offset().y * camera_speed * direction;

      camera_target = center;

    } else {

      std::cerr << "incorrect camera mode: " << camera.mode << std::endl;
      exit(1);
    }

    // Compute camera
    glm::mat4 view = projection * glm::lookAt(camera.pos, camera_target, camera.up);

    // Render scene!
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.render(obj, view);
    renderer.render(xyz, view);

    std::ostringstream str;
    str << "elapsed time (s): " << delta_time << '\n'
        << "camera mode: " << camera::modes_names[camera.mode] << '\n'
        << "camera base speed: " << camera.base_speed() << '\n'
        << "camera position: " << camera.pos.x << ","  << camera.pos.y << "," << camera.pos.z << '\n'
        << "fps: " << int(1.0f / delta_time);
    Text text = {str.str(), glm::vec2(0, 20), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), consolas_font};
    renderer.render(text);

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}
