#pragma once

#include <Geometry.h>
#include <vector>
#include <glm/glm.hpp>

class IndexedGeometry : public Geometry {
public:
    IndexedGeometry() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(3, vbos);
        glGenBuffers(1, &ebo);
    }

    ~IndexedGeometry() {
        glDeleteBuffers(3, vbos);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
    }
protected:
    virtual void generateGeometry() = 0;

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

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<unsigned int> indices;

private:
    GLuint vao;
    GLuint vbos[3];
    GLuint ebo;
};