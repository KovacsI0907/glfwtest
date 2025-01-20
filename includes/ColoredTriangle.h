#ifndef COLOREDTRIANGLE_H
#define COLOREDTRIANGLE_H

#include "TriangleGeometry.h"

class ColoredTriangle : public TriangleGeometry {
public:
    ColoredTriangle() {
        vertices = {
            -0.6f, -0.4f, 0.0f,
             0.6f, -0.4f, 0.0f,
             0.0f,  0.6f, 0.0f
        };

        uvs = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.5f, 1.0f
        };

        normals = {
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f
        };

        colors = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
        };

        setupBuffers();
    }

protected:
    std::vector<float> colors;

    void setupBuffers() override {
        bind();

        glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, vboUVs);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(2);

        GLuint vboColors;
        glGenBuffers(1, &vboColors);
        glBindBuffer(GL_ARRAY_BUFFER, vboColors);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(3);

        unbind();
    }
};

#endif // COLOREDTRIANGLE_H