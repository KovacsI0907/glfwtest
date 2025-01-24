#pragma once

#include "Geometry.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <vector>

class NonIndexedGeometry : public Geometry {

public:
    NonIndexedGeometry() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(4, vbos); // { changed code: now 4 buffers }
    }

    ~NonIndexedGeometry() {
        glDeleteBuffers(4, vbos); // { changed code: delete 4 buffers }
        glDeleteVertexArrays(1, &vao);
    }

protected:
    void bind() override {
        glBindVertexArray(vao);
    }

    void unbind() override {
        glBindVertexArray(0);
    }

    void uploadData() {
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(2);

        // { changed code: added tangents buffer }
        glBindBuffer(GL_ARRAY_BUFFER, vbos[3]);
        glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), tangents.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(3);
    }

    virtual void generateGeometry() = 0;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> tangents; // { changed code: new tangents field }

private:
    GLuint vao;
    GLuint vbos[4]; // { changed code: used 4 buffers instead of 3 }
};