#include "Shader.h"

Shader::Shader() : shaderID(0) {}

Shader::Shader(const std::filesystem::path& filePath, GLenum shaderType) : shaderID(0) {
    load(filePath);
    compile(shaderType);
}

Shader::~Shader() {
    cleanup();
}

Shader::Shader(Shader&& other) noexcept : shaderID(other.shaderID), shaderSource(std::move(other.shaderSource)) {
    other.shaderID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        cleanup();
        shaderID = other.shaderID;
        shaderSource = std::move(other.shaderSource);
        other.shaderID = 0;
    }
    return *this;
}

void Shader::load(const std::filesystem::path& filePath) {
    if (!std::filesystem::exists(filePath)) {
        throw std::runtime_error("Shader file does not exist: " + filePath.string());
    }

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + filePath.string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    shaderSource = buffer.str();

    if (shaderSource.empty()) {
        throw std::runtime_error("Shader file is empty: " + filePath.string());
    }

    name = filePath.filename().string();
}

void Shader::compile(GLenum shaderType) {
    if (shaderSource.empty()) {
        throw std::runtime_error("Shader source is empty. Load a shader file first.");
    }

    shaderID = glCreateShader(shaderType);
    const char* sourceCStr = shaderSource.c_str();
    glShaderSource(shaderID, 1, &sourceCStr, nullptr);
    glCompileShader(shaderID);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shaderID, sizeof(infoLog), nullptr, infoLog);
        throw std::runtime_error("Shader compilation failed (" + name + "): " + std::string(infoLog));
    }
}

GLuint Shader::getID() const {
    return shaderID;
}

void Shader::cleanup() {
    if (shaderID != 0) {
        glDeleteShader(shaderID);
        shaderID = 0;
    }
}
