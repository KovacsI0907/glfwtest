#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

out vec3 pos;
out vec3 normal;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(vPos, 1.0);
    pos = vPos;
    normal = vNormal;
    texCoord = vUV;
}