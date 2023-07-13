#include "view/Camera.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace gnev;

Camera::Camera(const std::shared_ptr<GladGLContext> &ctx) :
    buffer(ctx){
    buffer.glBufferStorage(16 * sizeof(float), nullptr, GL_DYNAMIC_STORAGE_BIT);
}

Camera::~Camera(){
}

void Camera::apply(){
    _forward = glm::mat4_cast(glm::quat({pitch, yaw, roll})) * glm::vec4(1, 0, 0, 0);
    auto mat = glm::lookAt(pos, pos + _forward, _up);
    if (use_perspective){
        auto proj_mat = glm::perspectiveFov(fov, static_cast<float>(width), static_cast<float>(height), near_z, far_z);
        mat = proj_mat * mat;
    }
    buffer.glBufferSubData(0, sizeof(glm::mat4), &mat);
}

// TODO angles
glm::mat4 Camera::applyLookAt(const glm::vec3& dst){
    auto mat = glm::lookAt(pos, dst, _up);
    if (use_perspective){
        auto proj_mat = glm::perspectiveFov(fov, static_cast<float>(width), static_cast<float>(height), near_z, far_z);
        mat = proj_mat * mat;
    }
    buffer.glBufferSubData(0, sizeof(glm::mat4), &mat);
    return mat;
}