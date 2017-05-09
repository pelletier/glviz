#include <iostream>

#include "gl.h"
#include "shader.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "glviz", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create visualization window." << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize glew." << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSetKeyCallback(window, key_callback);

    GLuint simple_vertex_shader;
    GLuint simple_fragment_shader;
    try {
        simple_vertex_shader = shader::load("simple.vert", GL_VERTEX_SHADER);
        simple_fragment_shader = shader::load("simple.frag", GL_FRAGMENT_SHADER);
    } catch (shader::LoadException e) {
        std::cerr << "shaders compilation failed: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, simple_vertex_shader);
    glAttachShader(shader_program, simple_fragment_shader);
    glLinkProgram(shader_program);

    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        constexpr size_t info_log_size = 1024;
        GLchar info_log[info_log_size];
        glGetProgramInfoLog(shader_program, info_log_size, NULL, info_log);
        std::cerr << "shader linking failed: " << '\n' << info_log << std::endl;
        return EXIT_FAILURE;
    }

    glDeleteShader(simple_vertex_shader);
    glDeleteShader(simple_fragment_shader);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLfloat vertices[] = {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
