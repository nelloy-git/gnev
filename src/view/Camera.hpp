#pragma once

#include "glm/glm.hpp"

#include "gl/BufferArrayCoherent.hpp"

namespace gnev {

struct CameraBuffer {
    glm::mat4 mat;
    glm::vec3 viewPos;
    glm::vec3 viewDir;
};

class EXPORT Camera {
public:
    Camera(const std::shared_ptr<GladGLContext> &ctx);
    virtual ~Camera();

    gl::BufferArrayCoherent<CameraBuffer> buffer;

    void apply();
    void applyLookAt(const glm::vec3& dst);

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
    glm::vec3 _forward = {1, 0, 0};
    glm::vec3 _up = {0, 1, 0};
};

}