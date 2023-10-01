#include "view/Camera.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace gnev;

Camera::Camera(const gl::Ctx& ctx)
    : buffer(ctx, STORAGE_FLAGS, 1, CameraBuffer{})
    , _map(static_cast<CameraBuffer*>(buffer.glMapBufferRange(0,
                                                              sizeof(CameraBuffer),
                                                              STORAGE_FLAGS))) {}

Camera::~Camera() {}

void Camera::apply() {
    CameraBuffer data;

    _forward = glm::mat4_cast(glm::quat({pitch, yaw, roll})) * glm::vec4(1, 0, 0, 0);

    data.viewPos = pos;
    data.viewDir = _forward;
    data.mat = glm::lookAt(pos, pos + _forward, _up);
    if (use_perspective) {
        auto proj_mat = glm::perspectiveFov(fov,
                                            static_cast<float>(width),
                                            static_cast<float>(height),
                                            near_z,
                                            far_z);
        data.mat = proj_mat * data.mat;
    }
    _map[0] = data;
}

// TODO angles
void Camera::applyLookAt(const glm::vec3& dst) {
    CameraBuffer data;

    _forward = glm::normalize(dst - pos);

    data.viewPos = pos;
    data.viewDir = _forward;
    data.mat = glm::lookAt(pos, dst, _up);
    if (use_perspective) {
        auto proj_mat = glm::perspectiveFov(fov,
                                            static_cast<float>(width),
                                            static_cast<float>(height),
                                            near_z,
                                            far_z);
        data.mat = proj_mat * data.mat;
    }
    _map[0] = data;
}