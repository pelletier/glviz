#version 330 core

in vec2 texture_coords;

uniform sampler2D tex;

out vec4 color;

void main() {
    //color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    color = texture(tex, texture_coords);
}