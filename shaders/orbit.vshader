#version 330 core
layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 coord;

void main()
{
    gl_Position = proj * view * model * vec4(pos, 1.0);
    coord = (model * vec4(pos, 1.0)).xyz;
}
