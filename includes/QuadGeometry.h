#pragma once

#include "glm/glm.hpp"
#include "NonIndexedGeometry.h"

class QuadGeometry : public NonIndexedGeometry {
public:
    QuadGeometry() {
        // Default quad centered at origin in XY plane
        verts[0] = glm::vec3(-0.5f, -0.5f, 0.0f);  // Bottom left
        verts[1] = glm::vec3(0.5f, -0.5f, 0.0f);   // Bottom right
        verts[2] = glm::vec3(-0.5f, 0.5f, 0.0f);   // Top left
        verts[3] = glm::vec3(0.5f, 0.5f, 0.0f);    // Top right
        generateGeometry();
        uploadData();
    }

protected:
    void generateGeometry() override {
        // Add vertices in order for triangle strip
        vertices.push_back(verts[0]); // Bottom left
        vertices.push_back(verts[1]); // Bottom right
        vertices.push_back(verts[2]); // Top left
        vertices.push_back(verts[1]); // Bottom right
        vertices.push_back(verts[2]); // Top left
        vertices.push_back(verts[3]); // Top right

        // Calculate normal (assuming counter-clockwise winding)
        glm::vec3 normal = glm::normalize(glm::cross(
            verts[1] - verts[0],  // Bottom edge
            verts[2] - verts[0]   // Left edge
        ));

        // Add same normal for all vertices
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);

        // Add UV coordinates
        uvs.push_back(glm::vec2(0.0f, 0.0f)); // Bottom left
        uvs.push_back(glm::vec2(1.0f, 0.0f)); // Bottom right
        uvs.push_back(glm::vec2(0.0f, 1.0f)); // Top left
        uvs.push_back(glm::vec2(1.0f, 0.0f)); // Bottom right
        uvs.push_back(glm::vec2(0.0f, 1.0f)); // Top left
        uvs.push_back(glm::vec2(1.0f, 1.0f)); // Top right

        tangents.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        tangents.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        tangents.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        tangents.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        tangents.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        tangents.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    }

    void drawOnly() override {
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

private:
    glm::vec3 verts[4];
};