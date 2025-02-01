#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;
layout(location = 3) in vec3 vTangent;

uniform mat4 M;
uniform mat3 N;
uniform mat4 VP;
uniform mat4 lightSpaceMatrix;

// Pass these to the fragment shader:
out vec3 worldPos;
out vec3 worldNormal;
out vec2 texCoord;
out vec3 worldTangent;
out vec3 posLightSpace;

out VS_OUT {
    vec3 normal;
} vs_out;

void main()
{
    // Existing MVP transform:
    gl_Position = (VP * M) * vec4(vPos, 1.0);

    worldPos = (M * vec4(vPos, 1.0)).xyz;
    worldNormal = normalize(N * vNormal);
    worldTangent = normalize(N * vTangent);

    vs_out.normal = normalize(N * vNormal);

    texCoord = vUV;

    vec4 posLightSpace4 = lightSpaceMatrix * M * vec4(vPos, 1.0);
    posLightSpace = posLightSpace4.xyz / posLightSpace4.w;
}