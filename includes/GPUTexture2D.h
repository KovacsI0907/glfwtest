#pragma once

#include "Texture2D.h"
#include <glad/glad.h>

class GPUTexture2D : Texture2D {
public:
    GPUTexture2D() {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void setMinFilter(GLint filter) {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    }

    void setMagFilter(GLint filter) {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    }

    GLuint getID() {
        return textureID;
    }

    virtual void bind() {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

	virtual void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    ~GPUTexture2D() {
        glDeleteTextures(1, &textureID);
    }

private:
    GLuint textureID;
};