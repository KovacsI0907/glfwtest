#pragma once

#include "UniformProvider.h"
#include <glm/glm.hpp>
#include <string>
#include <stdexcept>
#include <memory>
#include "Program.h"
#include <optional>
#include "GameObject.h"

class DirectionalLight : public UniformProvider {
public:
    static const int MAX_LIGHTS = 2;
    glm::vec3 direction;
    glm::vec3 color;
    const int index;

    DirectionalLight() : 
        direction(1.0f),
        color(1.0f),
        index(getNextIndex())
    {}

    DirectionalLight(const glm::vec3& dir, const glm::vec3& col) :
        direction(dir),
        color(col),
        index(getNextIndex())
    {}

    void uploadUniforms(std::shared_ptr<Program> program) override {
        program->setUniform("directionalLights[" + std::to_string(index) + "].direction", direction);
        program->setUniform("directionalLights[" + std::to_string(index) + "].color", color);
        program->setUniform("numDirectionalLights", currentIndex);
    }

    static void resetLightCount() {
        currentIndex = 0;
    }

private:
    static int currentIndex;

    static int getNextIndex() {
        if (currentIndex >= MAX_LIGHTS) {
            throw std::runtime_error("Exceeded maximum number of directional lights (" + std::to_string(MAX_LIGHTS) + ")");
        }
        return currentIndex++;
    }
};

int DirectionalLight::currentIndex = 0;