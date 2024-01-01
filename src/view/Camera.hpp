#pragma once

#include "gl/buffer/IndexMapViewElem.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "transform/mat/Mat4x4FStorage.hpp"
#include "util/Export.hpp"
#include "view/CameraDataGL.hpp"

namespace gnev {

class EXPORT Camera {
public:
    using DataGL = CameraGLdata;
    using DataView = gl::buffer::IndexMapView<DataGL>;
    using DataElem = gl::buffer::WeakIndexMapViewElem<DataGL>;

    static constexpr GLuint INVALID_INDEX = std::numeric_limits<GLuint>::max();

    Camera(const Ref<Mat4x4Storage>& matrix_storage);
    virtual ~Camera() = default;

    Ref<gl::Buffer> getBuffer() const;
    glm::mat4 getViewMat() const;
    GLuint getViewMatIndex() const;
    glm::mat4 getProjMat() const;
    GLuint getProjMatIndex() const;
    glm::vec3 getTop() const;

    void setPosition(const glm::vec3& position);
    glm::vec3 getPosition() const;

    void setDirection(const glm::vec3& direction);
    glm::vec3 getDirection() const;
    void lookAt(const glm::vec3& target);

private:
    Ref<DataView> data_view;
    Ref<DataElem> data_elem;
    Ref<Mat4x4Storage::DataElem> view_mat;
    Ref<Mat4x4Storage::DataElem> proj_mat;

    GLfloat fov = 2 * glm::pi<GLfloat>() / 3;
    GLfloat width = 1024;
    GLfloat height = 768;
    GLfloat near_z = 0.001;
    GLfloat far_z = 1000;

    GLfloat min_pitch = -0.9 * glm::pi<float>() / 2;
    GLfloat max_pitch = 0.9 * glm::pi<float>() / 2;

    void applyViewMat();
    void applyProjMat();
};

} // namespace gnev