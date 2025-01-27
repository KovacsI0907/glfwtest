#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
public:
    Transform()
        : rotation(glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 0.0f))), position(glm::vec3(0.0f)), scale(glm::vec3(1.0f)) {}

    // Translate the transform by a vector
    void translate(const glm::vec3& translation) {
        position += translation;
    }

    // Rotate the transform by a quaternion
    void rotate(const glm::vec3& axis, float angle) {
        rotation = glm::angleAxis(angle, glm::normalize(axis)) * rotation; // Apply the rotation
    }

    // Scale the transform
    void Scale(const glm::vec3& scaling) {
        scale *= scaling; // Element-wise scaling
    }

    // Rotate around a pivot point
    void rotateAroundPivot(const glm::vec3& pivot, const glm::vec3& axis, float angle) {
        glm::quat deltaRotation = glm::angleAxis(angle, axis);
        glm::vec3 direction = position - pivot;
        direction = deltaRotation * direction;
        position = pivot + direction;
        rotation = deltaRotation * rotation;
    }

    // Generate the model matrix
    glm::mat4 getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position); // Apply translation
        model *= glm::mat4_cast(rotation);          // Apply rotation
        model = glm::scale(model, scale);        // Apply scaling
        return model;
    }

    // Generate the normal matrix (for transforming normal vectors in shaders)
    glm::mat3 getNormalMatrix() const {
        // Get the model matrix
        glm::mat4 model = getModelMatrix();
        
        // Extract the upper-left 3x3 portion (no translation)
        glm::mat3 normalMatrix = glm::mat3(model);

        // Compute the inverse transpose for correct normal transformations
        normalMatrix = glm::inverse(glm::transpose(normalMatrix));

        return normalMatrix;
    }

    glm::quat rotation; // Stores rotation as a quaternion
    glm::vec3 position; // Stores the position of the transform
    glm::vec3 scale;    // Stores the scale of the transform
};