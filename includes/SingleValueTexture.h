#pragma once

#include "GPUTexture2D.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

class SingleValueTexture : public GPUTexture2D
{
public:
    SingleValueTexture(glm::vec4 value) {
        this->value = value;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, &value);
    }

	unsigned int getWidth() override {
		return 1;
	}

	unsigned int getHeight() override {
		return 1;
	}

	GLuint getID() {
		return textureID;
	}

	~SingleValueTexture() {
		glDeleteTextures(1, &textureID);
	}

private:
	GLuint textureID;
    glm::vec4 value;
};