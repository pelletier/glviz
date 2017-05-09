#pragma once

#include <string>
#include <stdexcept>

#include "gl.h"

namespace shader {

    class LoadException : public std::runtime_error {
    public:
        LoadException(std::string str) : std::runtime_error(str) {};
    };

    GLuint load(std::string shader_source_path, GLenum shader_type);
}
