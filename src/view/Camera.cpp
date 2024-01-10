#include "view/Camera.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/string_cast.hpp"

namespace gnev {

using DataView = Camera::DataView;
using DataElem = Camera::DataElem;

Camera::Camera(const Ref<Mat4x4Storage>& matrix_storage)
    : data_view(DataView::MakeCoherent(1))
    , data_elem(MakeSharable<DataElem>(data_view, DataGL{}))
    , view_mat(matrix_storage->lockMat(glm::mat4(1.f)))
    , proj_mat(matrix_storage->lockMat(glm::mat4(1.f))) {
    data_elem->set<GLuint>(view_mat->getIndex(),
                           offsetof(DataGL, mats) +
                               offsetof(decltype(DataGL::mats), view));
    data_elem->set<GLuint>(proj_mat->getIndex(),
                           offsetof(DataGL, mats) +
                               offsetof(decltype(DataGL::mats), proj));
    applyViewMat();
    applyProjMat();
}

Ref<gl::Buffer> Camera::getBuffer() const { return data_view->accessor->buffer; }

glm::mat4 Camera::getViewMat() const {
    glm::mat4 dst;
    view_mat->get(dst);
    return dst;
}

GLuint Camera::getViewMatIndex() const { return view_mat->getIndex(); }

glm::mat4 Camera::getProjMat() const {
    glm::mat4 dst;
    proj_mat->get(dst);
    return dst;
}

GLuint Camera::getProjMatIndex() const { return proj_mat->getIndex(); }

glm::vec3 Camera::getTop() const {
    glm::vec3 top;
    data_elem->get(top, offsetof(DataGL, top));
    return top;
}

void Camera::setPosition(const glm::vec3& position) {
    data_elem->set(position, offsetof(DataGL, position));
    applyViewMat();
}

glm::vec3 Camera::getPosition() const {
    glm::vec3 dst;
    data_elem->get(dst, offsetof(DataGL, position));
    return dst;
}

void Camera::setDirection(const glm::vec3& direction) {
    auto dir = glm::normalize(direction);
    auto pitch = glm::asin(dir.y);

    if (pitch <= min_pitch) {
        dir = glm::rotate(dir, min_pitch - pitch, glm::cross(dir, getTop()));
    }
    if (pitch >= max_pitch) {
        dir = glm::rotate(dir, max_pitch - pitch, glm::cross(dir, getTop()));
    }

    data_elem->set(dir, offsetof(DataGL, direction));
    applyViewMat();
}

glm::vec3 Camera::getDirection() const {
    glm::vec3 dst;
    data_elem->get(dst, offsetof(DataGL, direction));
    return dst;
}

void Camera::lookAt(const glm::vec3& target) {
    setDirection(glm::normalize(target - getPosition()));
    applyViewMat();
}

void Camera::applyViewMat() {
    DataGL cam;
    data_elem->get(cam);
    glm::mat4 mat = glm::lookAt(cam.position, cam.position + cam.direction, cam.top);
    view_mat->set(mat);
}

void Camera::applyProjMat() {
    DataGL cam;
    data_elem->get(cam);
    glm::mat4 mat = glm::perspectiveFov(fov, width, height, near_z, far_z);
    proj_mat->set(mat);
}

} // namespace gnev