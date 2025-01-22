#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Program.h"
#include <memory>

class UniformProvider {
public:
    virtual ~UniformProvider() = default;
    virtual void uploadUniforms(std::shared_ptr<Program> program) = 0;
};