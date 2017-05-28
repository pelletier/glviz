#include <iostream>
#include "Renderer.h"

void Renderer::render(const Drawable &drawable, const glm::mat4 &transfom) const {
  const shader::Shader& shader = drawable.shader();
  GLint transformLoc = glGetUniformLocation(shader.program(), "transform"); // TODO: make entry point more flexible
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transfom));
  shader.use();

  glBindVertexArray(drawable.vao());
  glDrawElements(drawable.polygon_type(), drawable.indices_count(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Renderer::render(const Text &text) {
  if (wireframe_mode) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  text_shader_.use();
  glUniform3f(glGetUniformLocation(text_shader_.program(), "text_color"), text.color.r, text.color.b, text.color.g);
  GLint transformLoc = glGetUniformLocation(text_shader_.program(), "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(overlay_projection_));

  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(text_vao_);

  GLfloat x = text.position.x;
  GLfloat y = text.position.y;
  const GLfloat delta_line = text.font.size * text.scale;
  for (const auto c : text.text) {
    if (c == '\n') {
      y += delta_line;
      x = text.position.x;
      continue;
    }
    const font::Character& ch = text.font.chars.at(c);

    GLfloat x_pos = x + ch.bearing.x * text.scale;
    GLfloat y_pos = screen_height_ - y - (ch.size.y - ch.bearing.y) * text.scale;
    GLfloat w = ch.size.x * text.scale;
    GLfloat h = ch.size.y * text.scale;

    // draw the character with two triangles to form a quad
    GLfloat vertices[6][4] = {
        { x_pos,     y_pos + h,   0.0, 0.0 },
        { x_pos,     y_pos,       0.0, 1.0 },
        { x_pos + w, y_pos,       1.0, 1.0 },

        { x_pos,     y_pos + h,   0.0, 0.0 },
        { x_pos + w, y_pos,       1.0, 1.0 },
        { x_pos + w, y_pos + h,   1.0, 0.0 }
    };
    glBindTexture(GL_TEXTURE_2D, ch.texture);

    glBindBuffer(GL_ARRAY_BUFFER, text_vbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    x += ch.advance * text.scale;
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);

  if (wireframe_mode) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
}

Renderer::Renderer(int screen_width, int screen_height)
    : screen_width_(screen_width)
    , screen_height_(screen_height)
    , text_shader_("text.vert", "text.frag") {
  overlay_projection_ = glm::ortho(0.0f, screen_width_, 0.0f, screen_height_); // puts the coords to bottom left corner

  // should be reused across text renders
  glGenVertexArrays(1, &text_vao_);
  glBindVertexArray(text_vao_);

  glGenBuffers(1, &text_vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, text_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  if (wireframe_mode) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
}
