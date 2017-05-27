#include <vector>
#include "Xyz.h"

Xyz::Xyz()
    : shader_("xyz.vert", "xyz.frag") {

  std::vector<GLfloat> vertices = {
      0.0f, 0.0f, 0.0f, // origin
      1.0f, 0.0f, 0.0f, // point at X
      0.0f, 0.0f, 0.0f, // origin
      0.0f, 1.0f, 0.0f, // point at Y
      0.0f, 0.0f, 0.0f, // origin
      0.0f, 0.0f, 1.0f, // point at Z
  };

  std::vector<GLuint> indices = {
      0, 1,
      2, 3,
      4, 5
  };

  // bind vao
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  // vertices
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);

  // colors

  std::vector<GLfloat> colors = {
      // X line red
      1.0f, 0.0f, 0.0f, // origin
      1.0f, 0.0f, 0.0f, // point at X
      // Y line green
      0.0f, 1.0f, 0.0f, // origin
      0.0f, 1.0f, 0.0f, // point at Y
      // Z line blue
      0.0f, 0.0f, 1.0f, // origin
      0.0f, 0.0f, 1.0f, // point at Z
  };


  GLuint colors_bo;
  glGenBuffers(1, &colors_bo);
  glBindBuffer(GL_ARRAY_BUFFER, colors_bo);
  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

  // elements index array
  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

  // unbind vao
  glBindVertexArray(0);
}

GLenum Xyz::polygon_type() const {
  return GL_LINES;
}

GLsizei Xyz::indices_count() const {
  return 6;
}

GLuint Xyz::vao() const {
  return vao_;
}

const shader::Shader &Xyz::shader() const {
  return shader_;
}
