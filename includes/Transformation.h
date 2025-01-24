#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>

class Transformation {
public:
    virtual glm::mat4 getModelMatrix() const = 0;
    virtual ~Transformation() = default;
};

class Rotation : public Transformation {
public:
    Rotation(glm::vec3 axis, float angle) : axis(axis), angle(angle) {}

    glm::mat4 getModelMatrix() const override{
        return glm::rotate(glm::mat4(1.0f), angle, axis);
    }

    glm::vec3 axis;
    float angle;
};

class Translation : public Transformation {
public:
    Translation(glm::vec3 position) : position(position) {}

    glm::mat4 getModelMatrix() const override{
        return glm::translate(glm::mat4(1.0f), position);
    }

    glm::vec3 position;
};

class Scale : public Transformation {
public:
    Scale(glm::vec3 scale) : scale(scale) {}

    glm::mat4 getModelMatrix() const override {
        return glm::scale(glm::mat4(1.0f), scale);
    }

    glm::vec3 scale;
};

class TransformationChain {
public:
    TransformationChain() : appliedTransformations(1.0f) {}

    void addTransformation(std::shared_ptr<Transformation> transformation) {
        transformations.push_back(transformation);
    }

    template<typename T>
    void addTransformation(T transformation) {
        static_assert(std::is_base_of<Transformation, T>::value, "T must be derived from Transformation");
        transformations.push_back(std::make_shared<T>(transformation));
    }

    glm::mat4 getModelMatrix() const {
        glm::mat4 modelMatrix = glm::mat4(1.0f) * appliedTransformations;
        for (auto& transformation : transformations) {
            modelMatrix *= transformation->getModelMatrix();
        }
        return modelMatrix;
    }

    glm::mat3 getNormalMatrix() const {
        glm::mat4 modelMatrix = getModelMatrix();
        return glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
    }

    void reset() {
        transformations.clear();
    }

    void resetAppliedTransformations() {
        appliedTransformations = glm::mat4(1.0f);
    }

    void apply() {
        appliedTransformations = getModelMatrix();
    }

private:
    std::vector<std::shared_ptr<Transformation>> transformations;
    glm::mat4 appliedTransformations;
};