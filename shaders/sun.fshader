#version 330 core

in vec3 coord;
in vec3 normal;
out vec4 frag_color;

void main() {
    frag_color = vec4(1.0, 0.25, 0.0, 1.0);
}
