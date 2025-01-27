#pragma once

#include "UniformProvider.h"
#include <glm/glm.hpp>
#include <string>
#include <stdexcept>
#include <memory>
#include "Program.h"
#include <optional>
#include "GameObject.h"

class PointLight : public UniformProvider {
public:
    static const int MAX_LIGHTS = 4;
    glm::vec3 position;
    glm::vec3 color;
    const int index;

    PointLight() : 
        position(0.0f),
        color(1.0f),
        index(getNextIndex())
    {}

    PointLight(const glm::vec3& pos, const glm::vec3& col) :
        position(pos),
        color(col),
        index(getNextIndex())
    {}

    void uploadUniforms(std::shared_ptr<Program> program) override {
        program->setUniform("lightPositions[" + std::to_string(index) + "]", position);
        program->setUniform("lightColors[" + std::to_string(index) + "]", color);
        program->setUniform("numLights", currentIndex);
    }

    static void resetLightCount() {
        currentIndex = 0;
    }

private:
    static int currentIndex;

    static int getNextIndex() {
        if (currentIndex >= MAX_LIGHTS) {
            throw std::runtime_error("Exceeded maximum number of lights (" + std::to_string(MAX_LIGHTS) + ")");
        }
        return currentIndex++;
    }
};

int PointLight::currentIndex = 0;