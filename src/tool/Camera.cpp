#include "tool/Camera.hpp"

#include <stdexcept>

#include "glm/ext/matrix_clip_space.hpp"
#include "tool/Transform.hpp"

namespace gnev::tool {

Camera::Camera() { apply(); }

void Camera::setFov(float fov) {
    fov_near_far_z[0] = fov;
    apply();
}

void Camera::setProjectionNearZ(float near_z) {
    fov_near_far_z[1] = near_z;
    apply();
}

void Camera::setProjectionFarZ(float far_z) {
    fov_near_far_z[2] = far_z;
    apply();
}

void Camera::setProjectionWidth(float width) {
    projection_size[0] = width;
    apply();
}

void Camera::setProjectionHeight(float height) { projection_size[1] = height; }

void Camera::apply() {
    switch (projection_type) {
        using enum ProjectionType;
    case ORTHO:
        projection_mat = glm::ortho(-(getProjectionWidth() / 2),
                                    getProjectionWidth(),
                                    -(getProjectionHeight() / 2),
                                    getProjectionHeight());
        return;
    case PERSPECTIVE:
        projection_mat = glm::perspectiveFov(getFov(),
                                             getProjectionWidth(),
                                             getProjectionHeight(),
                                             getProjectionNearZ(),
                                             getProjectionFarZ());
        return;
    default:
        throw std::out_of_range("");
    }
}

} // namespace gnev::tool