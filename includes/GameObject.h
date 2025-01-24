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
		mesh->getProgram()->use();
        uploadUniforms(mesh->getProgram());

        (providers.uploadUniforms(mesh->getProgram()), ...);

        if (mesh) {
            mesh->draw();
        }
    }

    void uploadUniforms(std::shared_ptr<Program> program) override {
        glm::mat4 modelMatrix = transformations.getModelMatrix();
        glm::mat3 normalMatrix = transformations.getNormalMatrix();
        
        program->setUniform("M", modelMatrix);
        program->setUniform("N", normalMatrix);
    }
public:
    TransformationChain transformations;
private:
    std::shared_ptr<Mesh> mesh;
};