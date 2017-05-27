#include <iostream>
#include "Freetype.h"

Freetype::Freetype() {
  if (FT_Init_FreeType(&ft_)) {
    std::cerr << "Could not init FreeType library." << std::endl;
    exit(EXIT_FAILURE); // TODO: more graceful exit
  }
}

Freetype::~Freetype() {
  FT_Done_FreeType(ft_);
}

const FT_Library Freetype::lib() const {
  return ft_;
}
