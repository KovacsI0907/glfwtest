#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
    glm::vec3 position;
    glm::mat4 rotationMatrix;
    glm::vec3 scale;

    Transform() 
        : position(0.0f, 0.0f, 0.0f), 
          rotationMatrix(1.0f), // Identity matrix
          scale(1.0f, 1.0f, 1.0f) {}

    void translate(const glm::vec3& t) {
        position += t;
    }

    void rotate(const glm::vec3& axis, float angle) {
        rotationMatrix = glm::rotate(rotationMatrix, angle, axis);
    }

    void Scale(const glm::vec3& s) {
        scale *= s;
    }

    glm::mat4 getModelMatrix() const {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    glm::mat4 getInverseModelMatrix() const {
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), 1.0f / scale);
        glm::mat4 rotationMatrixInverse = glm::transpose(rotationMatrix);
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -position);

        return scaleMatrix * rotationMatrixInverse * translationMatrix;
    }
};