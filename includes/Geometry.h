#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <glad/glad.h>

class Geometry {
public:
    Geometry() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vboVertices);
        glGenBuffers(1, &vboUVs);
        glGenBuffers(1, &vboNormals);
    }

    virtual ~Geometry() {
        glDeleteBuffers(1, &vboVertices);
        glDeleteBuffers(1, &vboUVs);
        glDeleteBuffers(1, &vboNormals);
        glDeleteVertexArrays(1, &vao);
    }

    void bind() const {
        glBindVertexArray(vao);
    }

    void unbind() const {
        glBindVertexArray(0);
    }

    virtual void draw() const = 0;

protected:
    GLuint vao;
    GLuint vboVertices;
    GLuint vboUVs;
    GLuint vboNormals;

    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;

    virtual void setupBuffers() = 0;
};

#endif // GEOMETRY_H