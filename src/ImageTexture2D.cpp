#include "ImageTexture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

ImageTexture2D::ImageTexture2D(std::filesystem::path path) : path(path) {}

void ImageTexture2D::load() {
    bind();
    stbi_set_flip_vertically_on_load(true);

    int nrChannels;
    unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        if (nrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else if (nrChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            throw ImageLoadException("Unsupported number of channels");
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        throw ImageLoadException("Failed to load texture");
    }
    stbi_image_free(data);
}