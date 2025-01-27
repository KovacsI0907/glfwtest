#pragma once

#include <vector>
#include <glad/glad.h>

class Geometry {
public:
    Geometry() {
    }
    
    virtual ~Geometry() {
    }

    void draw() {
        bind();
        drawOnly();
        unbind();
    }

protected:
    virtual void drawOnly() = 0;

    virtual void bind() = 0;
    virtual void unbind() = 0;
};