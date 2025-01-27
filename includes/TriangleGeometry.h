#pragma once

#include "NonIndexedGeometry.h"
#include "glm/glm.hpp"

class TriangleGeometry : public NonIndexedGeometry {
public:
    TriangleGeometry() {
        verts[0] = glm::vec3(0.0f, 0.0f, 0.0f);
        verts[1] = glm::vec3(1.0f, 0.0f, 0.0f);
        verts[2] = glm::vec3(0.0, 1.0f, 0.0f);
        generateGeometry();
        uploadData();
    }
    TriangleGeometry(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
        verts[0] = v0;
        verts[1] = v1;
        verts[2] = v2;
        generateGeometry();
        uploadData();
    }

protected:
    void generateGeometry() override {
        vertices.push_back(verts[0]);
        vertices.push_back(verts[1]);
        vertices.push_back(verts[2]);

        normals.push_back(glm::normalize(glm::cross(vertices[1] - vertices[0], vertices[2] - vertices[0])));
        normals.push_back(glm::normalize(glm::cross(vertices[1] - vertices[0], vertices[2] - vertices[0])));
        normals.push_back(glm::normalize(glm::cross(vertices[1] - vertices[0], vertices[2] - vertices[0])));

        uvs.push_back(glm::vec2(0.0f, 0.0f));
        uvs.push_back(glm::vec2(0.0f, 1.0f));
        uvs.push_back(glm::vec2(1.0f, 1.0f));
    }

    void drawOnly() override {
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

private:
    glm::vec3 verts[3];
};