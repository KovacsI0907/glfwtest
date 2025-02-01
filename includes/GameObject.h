#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "UniformProvider.h"
#include "Mesh.h"
#include "Transformation.h"
#include <Program.h>

class GameObject : public UniformProvider {
public:
    GameObject() = default;
    GameObject(std::shared_ptr<Mesh> mesh) : mesh(mesh) {}

    void setMesh(std::shared_ptr<Mesh> mesh) {
        this->mesh = mesh;
    }

    template<typename... UniformProviders>
    void draw(UniformProviders... providers) {
        uploadUniforms(mesh->getProgram());

        
        if (mesh) {
            mesh->draw(providers...);
        }
    }

    void depthMapDraw() {
        mesh->depthMapDraw(*this);
    }

    void uploadUniforms(std::shared_ptr<Program> program) override {
        glm::mat4 modelMatrix = transform.getModelMatrix();
        glm::mat3 normalMatrix = transform.getNormalMatrix();
        
        program->setUniform("M", modelMatrix);
        program->setUniform("N", normalMatrix);
    }
public:
    Transform transform;
private:
    std::shared_ptr<Mesh> mesh;
};