#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vCol;

uniform mat4 M;
uniform mat4 VP;

out vec3 color;

void main()
{
    gl_Position = VP * M * vec4(vPos, 1.0);
    color = vCol;
}