#include "Renderer.h"

void Renderer::render(const Drawable &drawable, const glm::mat4 &transfom) const {
    const shader::Shader& shader = drawable.shader();
    GLint transformLoc = glGetUniformLocation(shader.program(), "transform"); // TODO: make entry point more flexible
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transfom));
    shader.use();

    glBindVertexArray(drawable.vao());
    glDrawElements(drawable.polygon_type(), drawable.indices_count(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
