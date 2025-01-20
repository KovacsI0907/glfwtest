// Mesh.h
#pragma once

#include <memory>
#include "Geometry.h"
#include "Program.h"

class Mesh {
public:
    Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Program> program)
        : geometry(std::move(geometry)), program(std::move(program)) {}

    void draw() const {
        program->use();
        geometry->draw();
    }

    std::shared_ptr<Program> getProgram() const {
        return program;
    }

private:
    std::shared_ptr<Geometry> geometry;
    std::shared_ptr<Program> program;
};