#pragma once

#include "gl.h"
#include "Drawable.h"

/**
 * Display XYZ reference.
 */
class Xyz : public Drawable {
public:
    Xyz();

    // Implement the Drawable interface
    GLenum polygon_type() const override;

    GLsizei indices_count() const override;

    GLuint vao() const override;

    const shader::Shader &shader() const override;

private:
    GLuint vao_;
    shader::Shader shader_;
};
