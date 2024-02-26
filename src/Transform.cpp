#include "Transform.hpp"

#include "glm/ext/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/transform.hpp"

namespace gnev {

Transform::Transform(const Mat4x4& mat, const gl::HeapBufferRange<GL>& range)
    : mat{mat}
    , gl{range} {
    auto initial = Initial;
    initial.mat_index = mat.getIndex();
    gl.set(0, &initial);
}

unsigned Transform::getIndex() const { return gl.getOffset(); }

const Mat4x4& Transform::getMat() const { return mat; }

void Transform::setParent(const std::optional<Transform>& parent) {
    setParentIndex(parent->getIndex());
}

unsigned Transform::getParentIndex() const {
    unsigned parent_index;
    gl.getSub(0, &parent_index, offsetof(Transform::GL, parent_index));
    return parent_index;
}

void Transform::setParentIndex(unsigned parent_index) {
    gl.setSub(0, &parent_index, offsetof(Transform::GL, parent_index));
}

glm::vec3 Transform::getPosition() const { return position; }

void Transform::setPosition(const glm::vec3& pos) {
    position = pos;
    gl.setSub(0, &position, offsetof(Transform::GL, position));
    applyMat();
}

glm::vec3 Transform::getEuler() const { return euler; }

void Transform::setEuler(const glm::vec3& angles) {
    euler = angles;
    gl.setSub(0, &euler, offsetof(Transform::GL, euler));
    applyMat();
}

glm::vec3 Transform::getScale() const { return scale; }

void Transform::setScale(const glm::vec3& sc) {
    scale = sc;
    gl.setSub(0, &scale, offsetof(Transform::GL, scale));
    applyMat();
}

void Transform::applyMat() {
    auto glm_mat = glm::translate(position) *
                   glm::eulerAngleYXZ(euler.y, euler.x, euler.z) * glm::scale(scale);
    mat.set(glm_mat);
}

} // namespace gnev