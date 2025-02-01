#version 330 core
out vec4 FragColor;
uniform sampler2D image;

in vec3 pos;
in vec3 normal;
in vec2 texCoord;

void main()
{
    FragColor = texture(image, texCoord);
}
