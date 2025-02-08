#include "ImageTexture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

ImageTexture2D::ImageTexture2D(std::filesystem::path path, Type type) : path(path) {
    if(type == Type::Color) {
        setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
        setMagFilter(GL_LINEAR);
    } else if(type == Type::Normal) {
        setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
        setMagFilter(GL_LINEAR);
    } else if(type == Type::Data) {
        setMinFilter(GL_NEAREST);
        setMagFilter(GL_NEAREST);
    }
}

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
        throw ImageLoadException("Failed to load texture: " + path.string());
    }
    stbi_image_free(data);
}

ImageCubeMapTexture::ImageCubeMapTexture(const std::vector<std::filesystem::path> imagePaths) : paths(imagePaths) {
    if(imagePaths.size() != 6){
        throw std::runtime_error("Cubemap needs to have 6 images");
    }
}

void ImageCubeMapTexture::load() {
    bind();
    stbi_set_flip_vertically_on_load(false);

    int width, height, nrChannels;
    for (size_t i = 0; i < paths.size(); i++)
    {
        unsigned char *data = stbi_load(paths[i].string().c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            if (nrChannels == 3)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else if (nrChannels == 4)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            else
            {
                throw ImageLoadException("Unsupported number of channels");
            }
            //TODO do we need mipmaps?
            //glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            throw ImageLoadException("Failed to load texture: " + paths[i].string());
        }
        stbi_image_free(data);
    }
}