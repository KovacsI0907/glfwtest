#ifndef TRIANGLEGEOMETRY_H
#define TRIANGLEGEOMETRY_H

#include "Geometry.h"

class TriangleGeometry : public Geometry {
public:
    TriangleGeometry() {
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

        setupBuffers();
    }

    void draw() const override {
        bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        unbind();
    }

protected:
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

        unbind();
    }
};

#endif // TRIANGLEGEOMETRY_H