#pragma once

#include "gl.h"
#include "shader.h"

class Drawable {
public:
    virtual GLenum polygon_type() const = 0;
    virtual GLsizei indices_count() const = 0;
    virtual GLuint vao() const = 0;
    virtual const shader::Shader& shader() const = 0;
};
