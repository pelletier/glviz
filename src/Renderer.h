#pragma once

#include "glm.h"
#include "Drawable.h"
#include "Text.h"

class Renderer {
public:
  Renderer(int screen_width, int screen_height);
  bool wireframe_mode = false;

  /**
   * Render a Drawable on the scene, using the given world transform (usually camera).
   * The Drawable's shader must have a mat4 "transform" entry point.
   * @param drawable
   * @param transfom
   */
  void render(const Drawable &drawable, const glm::mat4 &transfom) const;
  void render(const Text& text);

private:
  shader::Shader text_shader_;
  glm::mat4 overlay_projection_;
  GLuint text_vao_;
  GLuint text_vbo_;
};

