#pragma once

#include "Shader.h"
#include <memory>
#include <stdexcept>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ImageTexture2D.h>

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

    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const glm::vec2& v);
    void setUniform(const std::string& name, const glm::vec3& v);
    void setUniform(const std::string& name, const glm::vec4& v);
    void setUniform(const std::string& name, const glm::mat3& mat);
    void setUniform(const std::string& name, const glm::mat4& mat);
    void setUniform(const std::string& name, ImageTexture2D& texture);
    void setUniform(const std::string& name, int num);

private:
    void cleanup();

    GLuint programID;
    bool isLinked;
    std::shared_ptr<Shader> vertexShader;
    std::shared_ptr<Shader> fragmentShader;
};
