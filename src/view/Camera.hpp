#pragma once

#include "gl/buffer/IndexMapViewElem.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "transform/mat/Mat4x4FStorage.hpp"
#include "util/Export.hpp"

namespace gnev {

struct EXPORT alignas(16) CameraGLdata final {
    CameraGLdata(GLuint view_mat_index, GLuint proj_mat_index);

    glm::vec3 position = {0, 0, 0};
    glm::vec3 direction = {1, 0, 0};
    glm::vec3 top = {0, 1, 0};

    struct {
        GLuint view_mat_index;
        GLuint proj_mat_index;
    } mats;
};

class EXPORT Camera {
public:
    using DataView = gl::buffer::IndexMapView<CameraGLdata>;
    using DataElem = gl::buffer::IndexMapViewElem<CameraGLdata>;

    static constexpr GLuint INVALID_INDEX = std::numeric_limits<GLuint>::max();

    Camera(const Ref<Mat4x4Storage>& mat_storage);
    virtual ~Camera() = default;

    Ref<gl::Buffer> getBuffer() const;

    void setPosition(const glm::vec3& position);
    glm::vec3 getPosition() const;

    void setDirection(const glm::vec3& direction);
    glm::vec3 getDirection() const;
    void lookAt(const glm::vec3& target);

    // void apply();
    // void applyLookAt(const glm::vec3& dst);

    // glm::vec3 pos = {0, 0, 0};
    // float yaw = 0;
    // float pitch = 0;
    // float roll = 0;

    // bool use_perspective = true;
    // float fov = float(2 * 3.14159265359 / 3);
    // float near_z = float(0.01);
    // float far_z = 10000;
    // int width = 1920;
    // int height = 1080;

private:
    Ref<Mat4x4Storage::DataElem> view_mat;
    Ref<Mat4x4Storage::DataElem> proj_mat;
    Ref<DataView> data_view;
    Ref<DataElem> data_elem;

    GLfloat fov = 2 * glm::pi<GLfloat>() / 3;
    GLfloat width = 1024;
    GLfloat height = 768;
    GLfloat near_z = 0.001;
    GLfloat far_z = 1000;

    void applyViewMat();
    void applyProjMat();
};

} // namespace gnev