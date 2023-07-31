#include "view/Camera.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace gnev;

Camera::Camera(const std::shared_ptr<GladGLContext> &ctx) :
    buffer(ctx){
    buffer.glBufferStorage(sizeof(CameraBuffer), nullptr, GL_DYNAMIC_STORAGE_BIT);
}

Camera::~Camera(){
}

void Camera::apply(){
    CameraBuffer data;

    _forward = glm::mat4_cast(glm::quat({pitch, yaw, roll})) * glm::vec4(1, 0, 0, 0);

    data.viewPos = pos;
    data.viewDir = _forward;
    data.mat = glm::lookAt(pos, pos + _forward, _up);
    if (use_perspective){
        auto proj_mat = glm::perspectiveFov(fov, static_cast<float>(width), static_cast<float>(height), near_z, far_z);
        data.mat = proj_mat * data.mat;
    }
    buffer.glBufferSubData(0, sizeof(CameraBuffer), &data);
}

// TODO angles
glm::mat4 Camera::applyLookAt(const glm::vec3& dst){
    CameraBuffer data;

    _forward = glm::normalize(dst - pos);

    data.viewPos = pos;
    data.viewDir = _forward;
    data.mat = glm::lookAt(pos, dst, _up);
    if (use_perspective){
        auto proj_mat = glm::perspectiveFov(fov, static_cast<float>(width), static_cast<float>(height), near_z, far_z);
        data.mat = proj_mat * data.mat;
    }
    buffer.glBufferSubData(0, sizeof(CameraBuffer), &data);
    return data.mat;
}