#pragma once

#include <string>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <glad/glad.h>

class Shader {
public:
    Shader();
    Shader(const std::filesystem::path& filePath, GLenum shaderType);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void load(const std::filesystem::path& filePath);
    void compile(GLenum shaderType);

    GLuint getID() const;

private:
    void cleanup();

    std::string shaderSource;
    GLuint shaderID;
    std::string name;
};
