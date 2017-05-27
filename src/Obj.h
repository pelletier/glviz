#pragma once

#include <string>
#include <vector>
#include "gl.h"
#include "Drawable.h"

/**
 * Wrapper around TinyObjLoader to read an obj file and load it as a VAO.
 */
class Obj : public Drawable {
public:
    Obj(std::string base, std::string file_name, shader::Shader &shader);

    // Implement Drawable interface
    GLenum polygon_type() const override;

    GLsizei indices_count() const override;

    GLuint vao() const override;

    const shader::Shader &shader() const override;

private:
    shader::Shader &shader_;
    GLuint vao_;
    GLsizei indices_size_;
};

