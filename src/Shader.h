#pragma once

#include <string>
#include <stdexcept>

#include "gl.h"

namespace shader {

    class ShaderException : public std::runtime_error {
    public:
        ShaderException(std::string str) : std::runtime_error("Shader: " + str) {};
    };

    class Shader {
    public:
        Shader(std::string vertex_source_path, std::string fragment_source_path);

        void use() const;

        GLuint program() const;

    private:
        GLuint program_;
    };
}
