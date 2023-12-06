#include "transform/3d/Transform_3D.hpp"

#include <iostream>

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/transform.hpp"

namespace gnev {

Transform_3D::Transform_3D(WeakRef<base::TransformStorage<TransformGL_3D>> weak_storage)
    : base::Transform<TransformGL_3D>(weak_storage) {}

std::optional<Ref<Transform_3D>> Transform_3D::getParentRef() const {
    return base::Transform<TransformGL_3D>::getParentRef()->dynamicCast<Transform_3D>();
}

void Transform_3D::setParentRef(const std::optional<Ref<Transform_3D>>& parent) {
    base::Transform<TransformGL_3D>::setParentRef(parent);
}

glm::mat4 Transform_3D::getMat() const {
    return glm::translate(position) * glm::eulerAngleYXZ(euler.y, euler.x, euler.z) *
           glm::scale(scale);
}

const glm::vec3& Transform_3D::getPosition() const { return position; }

void Transform_3D::setPosition(const glm::vec3& pos) {
    position = pos;
    apply();
}

const glm::vec3& Transform_3D::getEuler() const { return euler; }

void Transform_3D::setEuler(float yaw, float pitch, float roll) {
    euler = {pitch, yaw, roll};
    apply();
}

float Transform_3D::getYaw() const { return euler.y; }

void Transform_3D::setYaw(float yaw) {
    euler.y = yaw;
    apply();
}

float Transform_3D::getPitch() const { return euler.x; }

void Transform_3D::setPitch(float pitch) {
    euler.x = pitch;
    apply();
}

float Transform_3D::getRoll() const { return euler.z; }

void Transform_3D::setRoll(float roll) {
    euler.z = roll;
    apply();
}

const glm::vec3& Transform_3D::getScale() const { return scale; }

void Transform_3D::setScale(const glm::vec3& sc) {
    scale = sc;
    apply();
}

void Transform_3D::apply() {
    set(getMat(), offsetof(TransformGL_3D, mat));
}

} // namespace gnev