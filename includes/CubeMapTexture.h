#pragma once

#include <glad/glad.h>

class CubeMapTexture {
public:
    CubeMapTexture() {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    const GLuint getID() const {
        return id;
    }

    void bind() const {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    }

    void unbind() const {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    virtual ~CubeMapTexture() {
        glDeleteTextures(1, &id);
    }
private:
    GLuint id;
};