#pragma once

#include <glad/glad.h>
#include "FramebufferTexture.h"
#include <exception>

class Framebuffer {
public:
    static void bindDefault() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
public:
    Framebuffer(unsigned int width, unsigned int height) : targetTexture(width, height) {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, targetTexture.getID(), 0);

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Framebuffer is not complete");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    const FramebufferTexture2D& getTargetTexture() const {
        return targetTexture;
    }

    void bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

private:
    FramebufferTexture2D targetTexture;
    GLuint fbo;
    GLuint rbo;
};