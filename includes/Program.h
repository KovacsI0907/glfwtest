#pragma once

#include "Shader.h"
#include <memory>
#include <stdexcept>
#include <glad/glad.h>

class Program {
public:
    Program();
    Program(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader);
    ~Program();

    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;
    Program(Program&& other) noexcept;
    Program& operator=(Program&& other) noexcept;

    void attachShader(std::shared_ptr<Shader> shader);
    void link();
    void use() const;
    GLuint getID() const;

private:
    void cleanup();

    GLuint programID;
    bool isLinked;
    std::shared_ptr<Shader> vertexShader;
    std::shared_ptr<Shader> fragmentShader;
};
