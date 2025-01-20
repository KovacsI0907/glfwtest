#include "Program.h"

Program::Program() : programID(glCreateProgram()), isLinked(false) {
    if (programID == 0) {
        throw std::runtime_error("Failed to create OpenGL program.");
    }
}

Program::Program(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader)
    : Program() {
    attachShader(vertexShader);
    attachShader(fragmentShader);
    link();
}

Program::~Program() {
    cleanup();
}

Program::Program(Program&& other) noexcept
    : programID(other.programID), isLinked(other.isLinked),
      vertexShader(std::move(other.vertexShader)), fragmentShader(std::move(other.fragmentShader)) {
    other.programID = 0;
    other.isLinked = false;
}

Program& Program::operator=(Program&& other) noexcept {
    if (this != &other) {
        cleanup();
        programID = other.programID;
        isLinked = other.isLinked;
        vertexShader = std::move(other.vertexShader);
        fragmentShader = std::move(other.fragmentShader);
        other.programID = 0;
        other.isLinked = false;
    }
    return *this;
}

void Program::attachShader(std::shared_ptr<Shader> shader) {
    glAttachShader(programID, shader->getID());
    if (shader->getID() == GL_VERTEX_SHADER) {
        vertexShader = shader;
    } else if (shader->getID() == GL_FRAGMENT_SHADER) {
        fragmentShader = shader;
    }
}

void Program::link() {
    glLinkProgram(programID);

    // Check for linking errors
    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID, sizeof(infoLog), nullptr, infoLog);
        throw std::runtime_error("Program linking failed: " + std::string(infoLog));
    }

    isLinked = true;
}

void Program::use() const {
    glUseProgram(programID);
}

GLuint Program::getID() const {
    return programID;
}

void Program::cleanup() {
    if (programID != 0) {
        glDeleteProgram(programID);
        programID = 0;
    }
}
