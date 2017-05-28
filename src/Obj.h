#pragma once

#include <string>
#include <vector>
#include <map>
#include "gl.h"
#include "Shader.h"

/**
 * Wrapper around TinyObjLoader to read an obj file and load it as a VAO.
 */
class Obj {
public:
  Obj(std::string base, std::string file_name, shader::Shader &shader);

  shader::Shader& shader;
  std::vector<GLuint> vaos;
  std::vector<GLsizei> indices_counts;
  std::vector<GLuint> vao_to_texture;
  std::map<std::string, GLuint> textures;
};

