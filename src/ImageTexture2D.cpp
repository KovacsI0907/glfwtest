#include "ImageTexture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

ImageTexture2D::ImageTexture2D(std::filesystem::path path) : path(path) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void ImageTexture2D::load() {
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

    loaded = true;
}

void ImageTexture2D::bind() {
		if (!loaded) {
			throw std::runtime_error("Texture not loaded");
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	void ImageTexture2D::unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	unsigned int ImageTexture2D::getWidth() {
		if (!loaded) {
			throw std::runtime_error("Texture not loaded");
		}
		return width;
	}

	unsigned int ImageTexture2D::getHeight() {
		if(!loaded) {
			throw std::runtime_error("Texture not loaded");
		}
		return height;
	}

	GLuint ImageTexture2D::getID() {
		return textureID;
	}

	ImageTexture2D::~ImageTexture2D() {
		glDeleteTextures(1, &textureID);
	}