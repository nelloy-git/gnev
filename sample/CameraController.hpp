#pragma once

#include <functional>

#include "glm/glm.hpp"

#include "GlfwConveyor.hpp"
#include "gl/BufferArrayCoherent.hpp"

struct Camera {
    glm::mat4 view_mat;
    glm::mat4 projection_mat;
    glm::mat4 result_mat;
    glm::vec4 position;
    glm::vec4 direction;
    glm::vec4 top;
    GLfloat fov;
    GLfloat width;
    GLfloat height;
    GLfloat near_z;
    GLfloat far_z;
    GLfloat reserved_alignment[3];
};

class GlfwConveyor;

class CameraController {
public:
    CameraController(const std::shared_ptr<GladGLContext> &ctx);
    virtual ~CameraController();

    gnev::gl::BufferArrayCoherent<Camera>& buffer();
    const std::function<void(GlfwConveyor* conveyor, int key, int scancode, int action, int mods)> key_callback;

    GLfloat get_fov();
    void set_fov(GLfloat fov);

    GLfloat get_width();
    void set_width(GLfloat width);

    GLfloat get_height();
    void set_height(GLfloat height);

    GLfloat get_near_z();
    void set_near_z(GLfloat near_z);

    GLfloat get_far_z();
    void set_far_z(GLfloat far_z);

    const glm::vec4 get_position();
    void set_position(glm::vec4 position);

    const glm::vec4 get_direction();
    void set_direction(glm::vec4 direction);

    double get_sensitivity() const;
    void set_sensitivity(double sensitivity);

    void capture(GlfwConveyor& conveyor);


private:
    gnev::gl::BufferArrayCoherent<Camera> _camera;

    double _cursor_sensitivity = 0.015;
};