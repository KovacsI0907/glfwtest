#include "Program.h"
#include <glm/glm.hpp>

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

void Program::setUniform(const std::string& name, float value) {
    use();
    GLint location = glGetUniformLocation(programID, name.c_str());
    if (location >= 0) {
        glUniform1f(location, value);
    }
}

void Program::setUniform(const std::string& name, const glm::vec2& v) {
    use();
    GLint location = glGetUniformLocation(programID, name.c_str());
    if (location >= 0) {
        glUniform2f(location, v.x, v.y);
    }
}

void Program::setUniform(const std::string& name, const glm::vec3& v) {
    use();
    GLint location = glGetUniformLocation(programID, name.c_str());
    if (location >= 0) {
        glUniform3f(location, v.x, v.y, v.z);
    }
}

void Program::setUniform(const std::string& name, const glm::vec4& v) {
    use();
    GLint location = glGetUniformLocation(programID, name.c_str());
    if (location >= 0) {
        glUniform4f(location, v.x, v.y, v.z, v.w);
    }
}

void Program::setUniform(const std::string& name, const glm::mat3& mat) {
    use();
    GLint location = glGetUniformLocation(programID, name.c_str());
    if (location >= 0) {
        glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
    }
}

void Program::setUniform(const std::string& name, const glm::mat4& mat) {
    use();
    GLint location = glGetUniformLocation(programID, name.c_str());
    if (location >= 0) {
        glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
    }
}

void Program::setUniform(const std::string& name, const GPUTexture2D& texture, GLuint slot) {
    use();
    glActiveTexture(GL_TEXTURE0 + slot);
    texture.bind();
    GLint location = glGetUniformLocation(programID, name.c_str());
    if (location >= 0) {
        glUniform1i(location, slot);
    }
}

void Program::setUniform(const std::string& name, int num) {
    GLint location = glGetUniformLocation(programID, name.c_str());
    if (location >= 0) {
        glUniform1i(location, num);
    }
}
