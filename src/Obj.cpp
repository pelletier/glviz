#include <iostream>
#include "Obj.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "vendor/tiny_obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

// base must ends with a /
Obj::Obj(std::string base, std::string file_name, shader::Shader& shader) : shader(shader) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string err;
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, (base + file_name).c_str(), base.c_str(), true);

  if (!err.empty()) {
    std::cerr << "tinyobjloader: " << err << std::endl;
  }

  if (!ret) {
    throw std::runtime_error("tinyobjloader: could not load " + file_name);
  }

  // Load textures
  std::cout << "Loading " << materials.size() << " textures" << std::endl;
  for (const auto& m : materials) {
    const std::string& texture_name = m.diffuse_texname;
    if (texture_name.size() > 0) {
      std::cout << "Loading texture " << texture_name << " ";
      if (textures.find(texture_name) == textures.end()) {
        int width;
        int height;
        int components_count;
        unsigned char *image = stbi_load((base + texture_name).c_str(), &width, &height, &components_count,
                                         STBI_default);
        if (image == nullptr) {
          std::cerr << "failed" << std::endl;
          exit(1); // TODO: more graceful exit
        }
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        switch (components_count) {
          case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            break;
          case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            break;
          default:
            std::cerr << "unsupported components count: " << components_count << std::endl;
            exit(1); // TODO: better exit
            break;
        }
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(image);

        textures[texture_name] = texture;

        std::cout << "done" << std::endl;
      } else {
        std::cout << "skipped" << std::endl;
      }
    }
  }

  // Load triangles

  std::vector<GLfloat> vertices = attrib.vertices;
  std::vector<GLuint> indices;

  std::cout << "Vertices:";
  for (GLuint index = 0; index < vertices.size(); ++index) {
    if (index % 3 == 0) {
      std::cout << '\n' << index / 3 << "\t\t";
    }
    std::cout << vertices[index] << '\t';
  }
  std::cout << std::endl;


  std::map<int, std::vector<GLfloat>> vertex_attrs_per_mat;
  std::map<int, std::vector<GLuint>> vertex_index_per_mat;

  for (const tinyobj::shape_t &s : shapes) {
    std::cout << "Shape " << s.name << std::endl;
    for (size_t triangle_idx = 0; triangle_idx < s.mesh.indices.size() / 3; ++triangle_idx) {
      std::cout << "Triangle " << triangle_idx << std::endl;
      int material_id = s.mesh.material_ids[triangle_idx];
      // for each vertex of the triangle
      for (int i = 0; i < 3; ++i) {
        std::cout << "Vertex " << i << " ";
        const tinyobj::index_t &vertex_indices = s.mesh.indices[triangle_idx * 3 + i];

        vertex_attrs_per_mat[material_id].push_back(attrib.vertices[vertex_indices.vertex_index * 3 + 0]); // x
        vertex_attrs_per_mat[material_id].push_back(attrib.vertices[vertex_indices.vertex_index * 3 + 1]); // y
        vertex_attrs_per_mat[material_id].push_back(attrib.vertices[vertex_indices.vertex_index * 3 + 2]); // z

        vertex_attrs_per_mat[material_id].push_back(attrib.texcoords[vertex_indices.texcoord_index * 2 + 0]); // tex s
        vertex_attrs_per_mat[material_id].push_back(attrib.texcoords[vertex_indices.texcoord_index * 2 + 1]); // tex t

        vertex_index_per_mat[material_id].push_back((GLuint) vertex_index_per_mat[material_id].size());
      }
    }
  }

  for (const auto &entry : vertex_index_per_mat) {
    const std::vector<GLfloat> &attrs = vertex_attrs_per_mat[entry.first];

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, attrs.size() * sizeof(GLfloat), attrs.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    // elements index array
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, entry.second.size() * sizeof(GLuint), entry.second.data(), GL_STATIC_DRAW);

    // texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // unbind vao
    glBindVertexArray(0);

    std::cout << "Displaying VAO " << vao << std::endl;
    for (size_t i = 0; i < entry.second.size(); ++i) {
      GLuint idx = entry.second[i];
      std::cout << "i="  << i << " "
                << idx << " "
                << "(" << attrs[idx * 5 + 0] << "," << attrs[idx * 5 + 1] << "," << attrs[idx * 5 + 2] << ") "
                << "(" << attrs[idx * 5 + 3] << "," << attrs[idx * 5 + 4] << ")"
                << std::endl;
    }

    vaos.push_back(vao);
    indices_counts.push_back((GLsizei) entry.second.size());
    vao_to_texture.push_back(textures[materials[entry.first].diffuse_texname]);
  }
  std::cout << "Uploaded " << vaos.size() << " VAOs" << std::endl;
}
