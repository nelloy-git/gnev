#pragma once

#include <memory>

#include "glad/gl.h"
#include "glm/glm.hpp"

#include "gl/GLBuffer.hpp"

namespace gnev {

class __declspec(dllexport) Camera {
public:
    Camera(const std::shared_ptr<GladGLContext> &ctx);
    Camera(const Camera& other) = default;
    virtual ~Camera();

    GLBuffer buffer;

    void apply();
    glm::mat4 applyLookAt(const glm::vec3& dst);

    glm::vec3 pos = {0, 0, 0};
    float yaw = 0;
    float pitch = 0;
    float roll = 0;

    bool use_perspective = true;
    float fov = float(2 * 3.14159265359 / 3);
    float near_z = float(0.01);
    float far_z = 10000;
    int width = 1920;
    int height = 1080;

private:
    std::shared_ptr<GladGLContext> _ctx;

    glm::vec3 _forward = {1, 0, 0};
    glm::vec3 _up = {0, 1, 0};
};

}