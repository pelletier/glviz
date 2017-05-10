#pragma once

#include <string>
#include <vector>
#include "gl.h"

/**
 * Wrapper around TinyObjLoader to read an obj file and load it as a VAO.
 */
class Obj {
public:
    Obj(std::string base, std::string file_name);
    void draw() const;

private:
    GLuint vao_;
    GLsizei indices_size_;
};

