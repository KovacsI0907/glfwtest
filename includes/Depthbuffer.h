#pragma once

#include "DepthbufferTexture.h"

class Depthbuffer {
public:
    Depthbuffer(unsigned width, unsigned height) : targetTexture(width, height) {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, targetTexture.getID(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    const DepthbufferTexture2D& getTargetTexture() const {
        return targetTexture;
    }

    void bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }
private:
    DepthbufferTexture2D targetTexture;
    GLuint fbo;
};