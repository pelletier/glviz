#include <iostream>
#include "Obj.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

// base must ends with a /
Obj::Obj(std::string base, std::string file_name, shader::Shader& shader) : shader_(shader) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, (base + file_name).c_str(), base.c_str(), true); // convert polygons to triangles

    if (!err.empty()) {
        std::cerr << "tinyobjloader: " << err << std::endl;
    }

    if (!ret) {
        throw std::runtime_error("tinyobjloader: could not load " + file_name);
    }

    std::vector<GLfloat> vertices = attrib.vertices;
    std::vector<GLuint> indices;

    // FIXME: for now we assume just one shape
    for (const tinyobj::shape_t& s : shapes) {
        for (const tinyobj::index_t& idx : s.mesh.indices) {
            indices.push_back((GLuint)idx.vertex_index);
        }
    }
    indices_size_ = (GLsizei) indices.size();

    std::cout << "Vertices:";
    for (GLuint index = 0; index < vertices.size(); ++index) {
        if (index % 3 == 0) {
            std::cout << '\n' << index / 3 << "\t\t";
        }
        std::cout << vertices[index] << '\t';
    }
    std::cout << std::endl;


    std::cout << "Triangles:";
    for (GLuint index = 0; index < indices.size(); ++index) {
        if (index % 3 == 0) {
            std::cout << '\n' << index / 3 << "\t\t";
        }
        std::cout << indices[index] << '\t';
    }
    std::cout << std::endl;


    // bind vao
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // vertices
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    // elements index array
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // unbind vao
    glBindVertexArray(0);
}

GLenum Obj::polygon_type() const {
    return GL_TRIANGLES;
}

GLsizei Obj::indices_count() const {
    return indices_size_;
}

GLuint Obj::vao() const {
    return vao_;
}

const shader::Shader &Obj::shader() const {
    return shader_;
}
