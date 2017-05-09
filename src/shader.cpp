#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace shader {
    const std::string sources_root = "/Users/tpelletier/Code/pelletier/glviz/shaders/";

    GLuint load(std::string shader_source, GLenum shader_type) {
        const std::string shader_source_path = sources_root + shader_source;
        std::ifstream ifs(shader_source_path, std::ios::in | std::ios::binary);

        if (!ifs) {
            throw LoadException("shader: cannot open shader source file: " + shader_source_path);
        }

        std::ostringstream source_stream;
        source_stream << ifs.rdbuf();
        ifs.close();

        const std::string source = source_stream.str();
        const char* source_str = source.c_str();

        GLuint shader = glCreateShader(shader_type);
        glShaderSource(shader, 1, &source_str, NULL);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            constexpr size_t info_log_size = 1024;
            GLchar info_log[info_log_size];
            glGetShaderInfoLog(shader, info_log_size, NULL, info_log);
            std::cerr << "shader: cannot compile " << shader_source_path << '\n' << info_log << std::endl;
            throw LoadException("shader: cannot compile");
        }

        return shader;
    }
}
