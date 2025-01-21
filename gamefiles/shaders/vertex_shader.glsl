#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

uniform mat4 M;
uniform mat4 Minv;
uniform mat4 VP;

// Pass these to the fragment shader:
out vec3 worldPos;
out vec3 worldNormal;

void main()
{
    // Existing MVP transform:
    gl_Position = (VP * M) * vec4(vPos, 1.0);

    worldPos = (M * vec4(vPos, 1.0)).xyz;
    worldNormal = normalize((Minv * vec4(vNormal, 0.0)).xyz);
}