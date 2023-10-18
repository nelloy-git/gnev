#include "tool/Transform.hpp"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/transform.hpp"

namespace gnev::tool {

Transform::Transform() {}

Transform::Index Transform::getIndex() const { return node.index; }

void Transform::setParent(Index parent) { node.parent = parent; }

void Transform::setParent(const Transform& parent) { setParent(parent.node.index); }

void Transform::clearParent() { setParent(RESERVED_INDEX); }

Transform::Index Transform::getParentIndex() const { return node.parent; }

const glm::mat4& Transform::getMat() const { return mat; }

void Transform::setPos(const glm::vec3& position) {
    pos = position;
    apply();
}

const glm::vec3& Transform::getPos() const { return pos; }

void Transform::setAngles(float yaw, float pitch, float roll) {
    angles.x = pitch;
    angles.y = yaw;
    angles.z = roll;
    apply();
}

void Transform::setYaw(float yaw) {
    angles.y = yaw;
    apply();
}

void Transform::setPitch(float pitch) {
    angles.x = pitch;
    apply();
}

void Transform::setRoll(float roll) {
    angles.z = roll;
    apply();
}

float Transform::getYaw() const { return angles.y; }

float Transform::getPitch() const { return angles.x; }

float Transform::getRoll() const { return angles.z; }

void Transform::setScale(const glm::vec3& value) {
    scale = value;
    apply();
}

const glm::vec3& Transform::getScale() const { return scale; }

void Transform::apply() {
    mat = glm::translate(pos) * glm::eulerAngleYXZ(angles.y, angles.x, angles.z) *
          glm::scale(scale);
}

} // namespace gnev::tool