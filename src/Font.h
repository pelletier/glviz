#pragma once

#include <map>

#include "gl.h"
#include "glm.h"
#include "Freetype.h"

namespace font {
  struct Character {
    GLuint texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance; // in pixels
  };

  class Font {
  public:
    Font(const Freetype& ft, const std::string& font_path, GLuint size);
    GLuint size;
    std::map<GLchar, Character> chars;
  };
}

