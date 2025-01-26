#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "UniformProvider.h"
#include "DisplayInfo.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <memory>
#include "Program.h"
#include "Transformation.h"

class PerspectiveCamera : public UniformProvider {
public:
    glm::vec3 wLookat, wVup; // extrinsic parameters
    float fov, fp, bp; // intrinsic parameters
    std::shared_ptr<DisplayInfo> displayInfo;
    Transform transform;


    PerspectiveCamera(glm::vec3 lookat, glm::vec3 vup, float fovDeg, float nearPlane, float farPlane, std::shared_ptr<DisplayInfo> di)
        : wLookat(lookat), wVup(vup), fov(glm::radians(fovDeg)), fp(nearPlane), bp(farPlane), displayInfo(di) {
    }

    glm::vec3 wEye() {
        glm::vec4 eye = transform.getModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        return glm::vec3(eye.x, eye.y, eye.z) / eye.w;
    }

    glm::mat4 V() { // view matrix
        return glm::lookAt(wEye(), wLookat, wVup);
    }

    glm::mat4 P() { // projection matrix
        float asp = static_cast<float>(displayInfo->width) / displayInfo->height;
        return glm::perspective(fov, asp, fp, bp);
    }

    void uploadUniforms(std::shared_ptr<Program> program) override {
        program->setUniform("VP", P() * V());
        program->setUniform("camPosW", wEye());
    }
};