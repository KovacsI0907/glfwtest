#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "UniformProvider.h"
#include "DisplayInfo.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

class PerspectiveCamera : public UniformProvider {
public:
    glm::vec3 wEye, wLookat, wVup; // extrinsic parameters
    float fov, fp, bp; // intrinsic parameters
    std::shared_ptr<DisplayInfo> displayInfo;

    PerspectiveCamera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 vup, float fovDeg, float nearPlane, float farPlane, std::shared_ptr<DisplayInfo> di)
        : wEye(eye), wLookat(lookat), wVup(vup), fov(glm::radians(fovDeg)), fp(nearPlane), bp(farPlane), displayInfo(di) {
    }

    glm::mat4 V() { // view matrix
        return glm::lookAt(wEye, wLookat, wVup);
    }

    glm::mat4 P() { // projection matrix
        float asp = static_cast<float>(displayInfo->width) / displayInfo->height;
        return glm::perspective(fov, asp, fp, bp);
    }

    void uploadUniforms(GLuint programID) override {
        setUniform(programID, "VP", P() * V());
        setUniform(programID, "camPosW", wEye);
    }
};