#pragma once

#include "UniformProvider.h"
#include <glm/glm.hpp>

class PBRMaterial : public UniformProvider {
public:
    glm::vec3 albedo;
    float metallic;
    float roughness;
    float ao;

    PBRMaterial() : 
        albedo(1.0f, 1.0f, 1.0f),
        metallic(0.0f),
        roughness(0.5f),
        ao(1.0f) 
    {}

    ~PBRMaterial() override = default;

    void uploadUniforms(std::shared_ptr<Program> program) override {
        program->setUniform("albedo", albedo);
        program->setUniform("metallic", metallic);
        program->setUniform("roughness", roughness);
        program->setUniform("ao", ao);
    }
};