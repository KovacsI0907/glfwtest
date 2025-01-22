#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

uniform mat4 M;
uniform mat3 N;
uniform mat4 VP;

// Pass these to the fragment shader:
out vec3 worldPos;
out vec3 worldNormal;
out vec2 texCoord;

out VS_OUT {
    vec3 normal;
} vs_out;

void main()
{
    // Existing MVP transform:
    gl_Position = (VP * M) * vec4(vPos, 1.0);

    worldPos = (M * vec4(vPos, 1.0)).xyz;
    worldNormal = normalize(N * vNormal);

    vs_out.normal = normalize(N * vNormal);

    texCoord = vUV;
}