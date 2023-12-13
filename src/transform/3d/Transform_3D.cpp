#include "transform/3d/Transform_3D.hpp"

#include <iostream>

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/transform.hpp"

namespace gnev {

Transform_3D::Transform_3D(WeakRef<DataView> weak_view)
    : base::Transform<TransformGL_3D>(weak_view) {}

TransformGL_3D Transform_3D::get() const {
    TransformGL_3D result;
    data->get(result);
    return result;
}

void Transform_3D::setParent(const Ptr<Transform_3D>& parent) {
    base::Transform<TransformGL_3D>::setParent(parent);
}

Ptr<Transform_3D> Transform_3D::getParent() const {
    return std::dynamic_pointer_cast<
        Transform_3D>(base::Transform<TransformGL_3D>::getParent());
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

void Transform_3D::apply() { data->set(getMat(), offsetof(TransformGL_3D, mat)); }

} // namespace gnev