#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 _normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float rnd;

out vec3 coord;
out vec3 normal;

void main() {
    gl_Position = proj * view * model * vec4(pos, 1.0);
    coord = (model * vec4(pos, 1.0)).xyz;
    normal = _normal;
}
