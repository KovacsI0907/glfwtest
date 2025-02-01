#pragma once

#include "GPUTexture2D.h"
#include <glad/glad.h>

class DepthbufferTexture2D : public GPUTexture2D {
public:
    DepthbufferTexture2D(unsigned int width, unsigned int height) : width(width), height(height){
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
    }
private:
    unsigned int width;
    unsigned int height;
};