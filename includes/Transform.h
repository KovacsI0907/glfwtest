#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
    glm::vec3 position;
    glm::vec3 axis;
    float angle;
    glm::vec3 scale;

    Transform() 
        : position(0.0f, 0.0f, 0.0f), 
          axis(0.0f, 1.0f, 0.0f),
          angle(0.0f),
          scale(1.0f, 1.0f, 1.0f) {}

    void translate(const glm::vec3& t) {
        position += t;
    }

    void rotate(const glm::vec3& axis, float angle) {
        this->axis = axis;
        this->angle = angle;
    }

    void Scale(const glm::vec3& s) {
        scale *= s;
    }

    glm::mat4 getModelMatrix() const {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    glm::mat3 getNormalMatrix() const {
        glm::mat4 modelMatrix = getModelMatrix();
        return glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
    }
};