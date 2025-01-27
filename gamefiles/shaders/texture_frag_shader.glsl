#version 330 core
out vec4 FragColor;

uniform vec3 camPosW;
uniform sampler2D albedoMap;

in vec3 worldPos;
in vec3 worldNormal;
in vec2 texCoord;

void main()
{
    // Direction from fragment to camera
    vec3 viewDir = normalize(camPosW - worldPos);

    FragColor = texture(albedoMap, texCoord);
}
