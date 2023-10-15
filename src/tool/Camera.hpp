#pragma once

#include "glm/ext/scalar_constants.hpp"
#include "tool/Transform.hpp"

namespace gnev::tool {

class EXPORT alignas(16) Camera final {
public:
    using Index = Transform::Index;
    static constexpr Index RESERVED_INDEX = Transform::RESERVED_INDEX;

    enum class ProjectionType {
        ORTHO = 0,
        PERSPECTIVE = 1
    };

    Camera();
    ~Camera() = default;

    inline void setTransformIndex(Index transform_index);
    inline Index getTransformIndex() const;

    void setFov(float fov);
    inline float getFov() const;

    void setProjectionNearZ(float near_z);
    inline float getProjectionNearZ() const;

    void setProjectionFarZ(float far_z);
    inline float getProjectionFarZ() const;

    void setProjectionWidth(float width);
    inline float getProjectionWidth() const;

    void setProjectionHeight(float height);
    inline float getProjectionHeight() const;

private:
    Index transform_index = Transform::RESERVED_INDEX;
    ProjectionType projection_type = ProjectionType::PERSPECTIVE;
    glm::mat4 projection_mat;
    glm::vec3 fov_near_far_z = {glm::pi<float>(), 1, 100};
    glm::vec2 projection_size = {640, 480};

    void apply();
};

inline void Camera::setTransformIndex(Index transform_index_) {
    transform_index = transform_index_;
}

inline Camera::Index Camera::getTransformIndex() const { return transform_index; }

inline float Camera::getFov() const { return fov_near_far_z[0]; }

inline float Camera::getProjectionNearZ() const { return fov_near_far_z[1]; }

inline float Camera::getProjectionFarZ() const { return fov_near_far_z[2]; }

inline float Camera::getProjectionWidth() const { return projection_size[0]; }

inline float Camera::getProjectionHeight() const { return projection_size[1]; }

} // namespace gnev::tool