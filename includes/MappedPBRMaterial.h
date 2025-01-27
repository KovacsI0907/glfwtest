#pragma once

#include "GPUTexture2D.h"
#include "UniformProvider.h"
#include <memory>

class MappedPBRMaterial : UniformProvider
{
public:
    void uploadUniforms(std::shared_ptr<Program> program) override
    {
        // TODO dereferencing shared ptrs is not good
        program->setUniform("albedoMap", *albedo, 0);
        program->setUniform("metallicMap", *metallic, 1);
        program->setUniform("aoMap", *ao, 2);
        program->setUniform("roughnessMap", *roughness, 3);
        program->setUniform("normalMap", *normal, 4);
    }
    
    std::shared_ptr<GPUTexture2D> albedo;
    std::shared_ptr<GPUTexture2D> metallic;
    std::shared_ptr<GPUTexture2D> ao;
    std::shared_ptr<GPUTexture2D> roughness;
    std::shared_ptr<GPUTexture2D> normal;
};

    
    
