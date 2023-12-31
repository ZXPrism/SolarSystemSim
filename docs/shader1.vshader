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
    coord = pos;
    coord += _normal * pow((0.5 + 0.5 * sin(128 * coord.y * atan(coord.z / coord.x))), 64);
    gl_Position = proj * view * model * vec4(coord, 1.0);
    coord = (model * vec4(pos, 1.0)).xyz;
    normal = _normal;
}
