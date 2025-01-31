#pragma once

#include "GPUTexture2D.h"

class FramebufferTexture2D : public GPUTexture2D {
public:
    FramebufferTexture2D(unsigned int width, unsigned int height) : width(width), height(height){
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
private:
    unsigned int width;
    unsigned int height;
};