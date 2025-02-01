// Mesh.h
#pragma once

#include <memory>
#include <optional>
#include "Geometry.h"
#include "Program.h"
#include "UniformProvider.h"

class Mesh {
public:
    Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Program> program)
        : geometry(std::move(geometry)), program(std::move(program)), depthMapProgram(std::nullopt) {}

    Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Program> program, std::shared_ptr<Program> depthMapProgram)
        : geometry(std::move(geometry)), program(std::move(program)), depthMapProgram(depthMapProgram) {}

    template <typename... UniformProvider>
    void draw(UniformProvider... providers) const {
        (providers.uploadUniforms(program), ...);
        program->use();
        geometry->draw();
    }

    template <typename... UniformProvider>
    void depthMapDraw(UniformProvider... providers) const {
        if(depthMapProgram){
            (providers.uploadUniforms(*depthMapProgram), ...);
            (*depthMapProgram)->use();
            geometry->draw();
        }
    }

    std::shared_ptr<Program> getProgram() const {
        return program;
    }

    std::optional<std::shared_ptr<Program>> getDepthMapProgram() const {
        return depthMapProgram;
    }

private:
    std::shared_ptr<Geometry> geometry;
    std::shared_ptr<Program> program;
    std::optional<std::shared_ptr<Program>> depthMapProgram;
};