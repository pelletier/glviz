#pragma once

#include "glm.h"
#include "Font.h"

struct Text {
  std::string text;
  glm::vec2 position;
  GLfloat scale;
  glm::vec3 color;
  const font::Font& font;
};

