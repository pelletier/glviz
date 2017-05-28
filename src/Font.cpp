#include <iostream>
#include "Font.h"

font::Font::Font(const Freetype& ft, const std::string& font_path, GLuint size) {
  std::cout << "Loading font " << font_path << std::endl;
  FT_Face face;
  if (FT_New_Face(ft.lib(), font_path.c_str(), 0, &face)) {
    std::cerr << "Could not open font " << font_path << std::endl;
    exit(EXIT_FAILURE); // TODO: more graceful exit
  }

  FT_Set_Pixel_Sizes(face, 0, size);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Create textures for the first 128 characters
  for (GLuint c = 0; c < 128; ++c) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cerr << "Could not load glyph 0x" << std::hex << c << " from font " << font_path << std::endl;
      continue;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        ((GLuint) face->glyph->advance.x) >> 6 // advanced is expressed in 1/64th of a pixel, so convert it to pixel
    };

    chars.insert(std::pair<GLchar, Character>(c, character));
  }

  FT_Done_Face(face);
}
