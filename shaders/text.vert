#version 330 core


layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 texture>
out vec2 texture_coords;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(vertex.xy, 0.0, 1.0); // project flat on the screen
    texture_coords = vertex.zw;
}
