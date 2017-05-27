#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace shader {
    const std::string sources_root = "/Users/tpelletier/Code/pelletier/glviz/shaders/";

    GLuint load(std::string shader_source, GLenum shader_type) {
      const std::string shader_source_path = sources_root + shader_source;
      std::ifstream ifs(shader_source_path, std::ios::in | std::ios::binary);

      if (!ifs) {
        throw ShaderException("cannot open Shader source file: " + shader_source_path);
      }

      std::ostringstream source_stream;
      source_stream << ifs.rdbuf();
      ifs.close();

      const std::string source = source_stream.str();
      const char *source_str = source.c_str();

      GLuint shader = glCreateShader(shader_type);
      glShaderSource(shader, 1, &source_str, NULL);
      glCompileShader(shader);

      GLint success;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

      if (!success) {
        constexpr size_t info_log_size = 1024;
        GLchar info_log[info_log_size];
        glGetShaderInfoLog(shader, info_log_size, NULL, info_log);
        std::cerr << "Shader: cannot compile " << shader_source_path << '\n' << info_log << std::endl;
        throw ShaderException("cannot compile");
      }

      return shader;
    }

    Shader::Shader(std::string vertex_source_path, std::string fragment_source_path) {
      std::cout << "Loading and compiling Shader: vertex=" << vertex_source_path << " fragment=" << fragment_source_path
                << std::endl;
      GLuint vertex_shader;
      GLuint fragment_shader;
      try {
        vertex_shader = load(vertex_source_path, GL_VERTEX_SHADER);
        fragment_shader = load(fragment_source_path, GL_FRAGMENT_SHADER);
      } catch (ShaderException e) {
        std::cerr << "compilation failed: " << e.what() << std::endl;
        throw e;
      }

      program_ = glCreateProgram();
      glAttachShader(program_, vertex_shader);
      glAttachShader(program_, fragment_shader);
      glLinkProgram(program_);

      GLint success;
      glGetProgramiv(program_, GL_LINK_STATUS, &success);
      if (!success) {
        constexpr size_t info_log_size = 1024;
        GLchar info_log[info_log_size];
        glGetProgramInfoLog(program_, info_log_size, NULL, info_log);
        std::cerr << "Shader linking failed: " << '\n' << info_log << std::endl;
        throw ShaderException("linking failed");
      }

      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
    }

    void Shader::use() const {
      glUseProgram(program_);
    }

    GLuint Shader::program() const {
      return program_;
    }
}
