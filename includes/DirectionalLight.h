#pragma once

#include "UniformProvider.h"
#include <glm/glm.hpp>
#include "Program.h"

class DirectionalLight : public UniformProvider {
public:
    glm::vec3 direction;
    glm::vec3 color;

    DirectionalLight() : 
        direction(1.0f),
        color(1.0f)
    {}

    DirectionalLight(const glm::vec3& dir, const glm::vec3& col) :
        direction(dir),
        color(col)
    {}

    void uploadUniforms(std::shared_ptr<Program> program) override {
        program->setUniform("directionalLight.direction", direction);
        program->setUniform("directionalLight.color", color);
    }
};