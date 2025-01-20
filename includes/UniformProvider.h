#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class UniformProvider {
public:
    virtual ~UniformProvider() = default;
    virtual void uploadUniforms(GLuint programID) = 0;

    // Upload a single float uniform
    static void setUniform(GLuint programID, const std::string& name, float value) {
        GLint location = glGetUniformLocation(programID, name.c_str());
        if (location >= 0) {
            glUniform1f(location, value);
        }
    }

    // Upload a vec2 uniform
    static void setUniform(GLuint programID, const std::string& name, const glm::vec2& v) {
        GLint location = glGetUniformLocation(programID, name.c_str());
        if (location >= 0) {
            glUniform2f(location, v.x, v.y);
        }
    }

    // Upload a vec3 uniform
    static void setUniform(GLuint programID, const std::string& name, const glm::vec3& v) {
        GLint location = glGetUniformLocation(programID, name.c_str());
        if (location >= 0) {
            glUniform3f(location, v.x, v.y, v.z);
        }
    }

    // Upload a vec4 uniform
    static void setUniform(GLuint programID, const std::string& name, const glm::vec4& v) {
        GLint location = glGetUniformLocation(programID, name.c_str());
        if (location >= 0) {
            glUniform4f(location, v.x, v.y, v.z, v.w);
        }
    }

    // Upload a mat4 uniform
    static void setUniform(GLuint programID, const std::string& name, const glm::mat4& mat) {
        GLint location = glGetUniformLocation(programID, name.c_str());
        if (location >= 0) {
            glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
        }
    }
};