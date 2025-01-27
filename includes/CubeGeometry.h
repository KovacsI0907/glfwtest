#pragma once
#include "IndexedGeometry.h"
#include "glm/glm.hpp"

class CubeGeometry : public IndexedGeometry {
public:
    CubeGeometry() {
        generateGeometry();
        uploadData();
    }

protected:
    void generateGeometry() override {
        // 8 corners of a cube centered at origin (size 1)
        vertices = {
            glm::vec3(-0.5f, -0.5f, -0.5f),
            glm::vec3(-0.5f, -0.5f,  0.5f),
            glm::vec3(-0.5f,  0.5f,  0.5f),
            glm::vec3(-0.5f,  0.5f, -0.5f),
            glm::vec3( 0.5f, -0.5f, -0.5f),
            glm::vec3( 0.5f, -0.5f,  0.5f),
            glm::vec3( 0.5f,  0.5f,  0.5f),
            glm::vec3( 0.5f,  0.5f, -0.5f)
        };

        // 12 triangles (36 indices)
        indices = {
            // front
            1, 5, 6,  1, 6, 2,
            // back
            0, 3, 7,  0, 7, 4,
            // left
            0, 1, 2,  0, 2, 3,
            // right
            4, 6, 5,  4, 7, 6,
            // top
            3, 2, 6,  3, 6, 7,
            // bottom
            0, 4, 5,  0, 5, 1
        };

        // Prepare normals
        normals.resize(vertices.size(), glm::vec3(0.0f));
        // Accumulate face normals
        for (size_t i = 0; i < indices.size(); i += 3) {
            auto i0 = indices[i + 0];
            auto i1 = indices[i + 1];
            auto i2 = indices[i + 2];
            glm::vec3 n = glm::normalize(glm::cross(
                vertices[i1] - vertices[i0],
                vertices[i2] - vertices[i0]
            ));
            normals[i0] += n;
            normals[i1] += n;
            normals[i2] += n;
        }
        // Normalize accumulated normals
        for (auto &n : normals) {
            n = glm::normalize(n);
        }

        // Trivial UV setup
        uvs.resize(vertices.size(), glm::vec2(0.0f));
    }

    void drawOnly() override {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    }
};