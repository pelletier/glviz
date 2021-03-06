#pragma once

#include "gl.h"
#include "glm.h"

class Mouse {
public:
  static Mouse &global_instance();

  static void bind_glfw_callbacks(GLFWwindow *window);

  void glfw_mouse_movement_callback(double xpos, double ypos);
  void glfw_mouse_button_callback(int button, int action, int mode);
  void glfw_mouse_scroll_callback(double xoffset, double yoffset);

  const glm::vec2 get_offset();
  const glm::vec2 get_scroll_offset();

  bool is_right_button_down() const;

private:
  bool first_pos_ = false;
  bool right_button_down_ = false;
  GLfloat sensitivity_ = 0.1f;
  glm::vec2 last_pos_;
  glm::vec2 current_pos_;
  glm::vec2 scroll_offset_;
};
