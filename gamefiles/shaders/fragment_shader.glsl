#version 330 core
out vec4 FragColor;

uniform vec3 color;
uniform vec3 camPosW; // camera position in world space

in vec3 worldPos;
in vec3 worldNormal;

void main()
{
    // Direction from fragment to camera
    vec3 viewDir = normalize(camPosW - worldPos);

    float nDotV = dot(normalize(worldNormal), viewDir);
    float brightness = abs(nDotV);

    FragColor = vec4(color, 1.0) * brightness;
}
