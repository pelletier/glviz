#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

class Freetype {
public:
  Freetype();
  ~Freetype();
  const FT_Library lib() const;

private:
  FT_Library ft_;
};