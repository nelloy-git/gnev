#include "view/Camera.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace gnev {

using DataView = Camera::DataView;
using DataElem = Camera::DataElem;

CameraGLdata::CameraGLdata(GLuint view_mat_index, GLuint proj_mat_index)
    : mats{.view_mat_index = view_mat_index, .proj_mat_index = proj_mat_index} {}

Camera::Camera(const Ref<Mat4x4Storage>& mat_storage)
    : view_mat(mat_storage->lockMat(glm::mat4(1.f)))
    , proj_mat(mat_storage->lockMat(glm::mat4(1.f)))
    , data_view(DataView::MakeCoherent(1))
    , data_elem(MakeSharable<DataElem>(data_view,
                                       CameraGLdata{view_mat->getIndex(),
                                                    proj_mat->getIndex()})) {}

Ref<gl::Buffer> Camera::getBuffer() const { return data_view->accessor->buffer; }

void Camera::setPosition(const glm::vec3& position) {
    data_elem->set(position, offsetof(CameraGLdata, position));
    applyViewMat();
}

glm::vec3 Camera::getPosition() const {
    glm::vec3 dst;
    data_elem->get(dst, offsetof(CameraGLdata, position));
    return dst;
}

void Camera::setDirection(const glm::vec3& direction) {
    data_elem->set(direction, offsetof(CameraGLdata, direction));
    applyViewMat();
}

glm::vec3 Camera::getDirection() const {
    glm::vec3 dst;
    data_elem->get(dst, offsetof(CameraGLdata, direction));
    return dst;
}

void Camera::lookAt(const glm::vec3& target) {
    data_elem->set(glm::normalize(target - getPosition()),
                   offsetof(CameraGLdata, direction));
    applyViewMat();
}

void Camera::applyViewMat() {
    CameraGLdata cam{INVALID_INDEX, INVALID_INDEX};
    data_elem->get(cam);
    glm::mat4 mat = glm::lookAt(cam.position, cam.position + cam.direction, cam.top);
    view_mat->set(mat);
}

void Camera::applyProjMat() {
    CameraGLdata cam{INVALID_INDEX, INVALID_INDEX};
    data_elem->get(cam);
    glm::mat4 mat = glm::perspectiveFov(fov, width, height, near_z, far_z);
    proj_mat->set(mat);
}

} // namespace gnev