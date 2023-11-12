#version 330 core

in vec3 coord;
in vec3 normal;
out vec4 frag_color;

uniform vec3 color;

vec3 light_pos;

float ambient = 0.1;

void main() {
    light_pos = vec3(0.0, 0.0, 0.0);
    vec3 light = normalize(light_pos - coord);
    float diffuse = max(dot(light, normal), 0.0);
    frag_color = min((ambient + diffuse), 1.0) * vec4(color, 1.0);
}
