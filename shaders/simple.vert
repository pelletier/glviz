#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coords;

out vec2 texture_coords;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(position, 1.0);
    texture_coords = tex_coords;
}