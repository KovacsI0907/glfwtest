#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "UniformProvider.h"
#include "Mesh.h"
#include "Transform.h"

class GameObject : public UniformProvider {
public:
    GameObject() = default;
    GameObject(std::shared_ptr<Mesh> mesh) : mesh(mesh) {}

    void setMesh(std::shared_ptr<Mesh> mesh) {
        this->mesh = mesh;
    }

    void setTransform(const Transform& transform) {
        this->transform = transform;
    }

    template<typename... UniformProviders>
    void draw(UniformProviders... providers) {
		mesh->getProgram()->use();
        uploadUniforms(mesh->getProgram()->getID());
        (providers.uploadUniforms(mesh->getProgram()->getID()), ...);
        if (mesh) {
            mesh->draw();
        }
    }

    void uploadUniforms(GLuint programID) override {
        glm::mat4 modelMatrix = transform.getModelMatrix();
        glm::mat4 modelInverseMatrix = transform.getInverseModelMatrix();
        
        setUniform(programID, "M", modelMatrix);
        setUniform(programID, "Minv", modelInverseMatrix);
    }
public:
    Transform transform;
private:
    std::shared_ptr<Mesh> mesh;
};