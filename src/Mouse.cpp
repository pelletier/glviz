#include "Mouse.h"

/**
   GLFW callbacks for mouse actions.
*/
void mouse_movement_glfw_callback(GLFWwindow* window, double xpos, double ypos) {
  Mouse::global_instance().glfw_mouse_movement_callback(xpos, ypos);
}

void mouse_button_glfw_callback(GLFWwindow* window, int button, int action, int mode) {
  Mouse::global_instance().glfw_mouse_button_callback(button, action, mode);
}

void Mouse::bind_glfw_callbacks(GLFWwindow *window) {
  glfwSetCursorPosCallback(window, mouse_movement_glfw_callback);
  glfwSetMouseButtonCallback(window, mouse_button_glfw_callback);
}

Mouse& Mouse::global_instance() {
  static Mouse global_instance;
  return global_instance;
}

void Mouse::glfw_mouse_button_callback(int button, int action, int mode) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    switch (action) {
    case GLFW_PRESS:
      right_button_down_ = true;
      first_pos_ = true;
      break;
    case GLFW_RELEASE:
      right_button_down_ = false;
      break;
    default:
      break;
    }
  }
}

void Mouse::glfw_mouse_movement_callback(double xpos, double ypos) {
  current_pos_.x = xpos;
  current_pos_.y = ypos;
}

const glm::vec2 Mouse::get_offset(void) {
  if (first_pos_) {
    first_pos_ = false;
    last_pos_ = current_pos_;
  }

  glm::vec2 offset = (current_pos_ - last_pos_) * sensitivity_;

  if (true) { // TODO: create setting for inverted Y axis
    offset.y *= -1;
  }

  last_pos_ = current_pos_;
  return offset;
}

bool Mouse::is_right_button_down(void) const {
  return right_button_down_;
}
