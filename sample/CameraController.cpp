#include "CameraController.hpp"

#include <iostream>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

namespace {
    constexpr Camera default_camera {
        .view_mat = {1, 0, 0, 0,
                     0, 1, 0, 0,
                     0, 0, 1, 0,
                     0, 0, 0, 1},
        .projection_mat = {1, 0, 0, 0,
                           0, 1, 0, 0,
                           0, 0, 1, 0,
                           0, 0, 0, 1},
        .result_mat = {1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1},
        .position = {0, 0, 0, 0},
        .direction = {1, 0, 0, 0},
        .top = {0, 1, 0, 0},
        .fov = 3.14159265359 / 2,
        .width = 1920,
        .height = 1080,
        .near_z = 0.01,
        .far_z = 1000,
        .reserved_alighment = {0, 0, 0},
    };
}

CameraController::CameraController(const std::shared_ptr<GladGLContext> &ctx)
    : _camera(ctx, &default_camera, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT),
      _map(_camera.glMapBufferRange(GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT))
{
    _map->position = glm::vec4{1, 1, 1, 0};
    _map->direction = glm::normalize(glm::vec4{-1, -1, -1, 0});
    _map->view_mat = glm::lookAt(glm::vec3{1, 1, 1}, {0, 0, 0}, {0, 1, 0});
    _map->projection_mat = glm::perspectiveFov(_map->fov, static_cast<float>(_map->width), static_cast<float>(_map->height), _map->near_z, _map->far_z);
    _map->result_mat = _map->projection_mat * _map->view_mat;
}

CameraController::~CameraController()
{
    _camera.glUnmapBuffer();
}

gnev::GLBuffer& CameraController::buffer()
{
    return _camera;
}

GLfloat CameraController::get_fov()
{
    return _map->fov;
}

void CameraController::set_fov(GLfloat fov)
{
    _map->fov = fov;
    _map->projection_mat = glm::perspectiveFov(_map->fov, static_cast<float>(_map->width), static_cast<float>(_map->height), _map->near_z, _map->far_z);
    _map->result_mat = _map->projection_mat * _map->view_mat;
}

GLfloat CameraController::get_width()
{
    return _map->width;
}

void CameraController::set_width(GLfloat width)
{
    _map->width = width;
    _map->projection_mat = glm::perspectiveFov(_map->fov, static_cast<float>(_map->width), static_cast<float>(_map->height), _map->near_z, _map->far_z);
    _map->result_mat = _map->projection_mat * _map->view_mat;
}

GLfloat CameraController::get_height()
{
    return _map->height;
}

void CameraController::set_height(GLfloat height)
{
    _map->height = height;
    _map->projection_mat = glm::perspectiveFov(_map->fov, static_cast<float>(_map->width), static_cast<float>(_map->height), _map->near_z, _map->far_z);
    _map->result_mat = _map->projection_mat * _map->view_mat;
}

GLfloat CameraController::get_near_z()
{
    return _map->near_z;
}

void CameraController::set_near_z(GLfloat near_z)
{
    _map->near_z = near_z;
    _map->projection_mat = glm::perspectiveFov(_map->fov, static_cast<float>(_map->width), static_cast<float>(_map->height), _map->near_z, _map->far_z);
    _map->result_mat = _map->projection_mat * _map->view_mat;
}

GLfloat CameraController::get_far_z()
{
    return _map->far_z;
}

void CameraController::set_far_z(GLfloat far_z)
{
    _map->far_z = far_z;
    _map->projection_mat = glm::perspectiveFov(_map->fov, static_cast<float>(_map->width), static_cast<float>(_map->height), _map->near_z, _map->far_z);
    _map->result_mat = _map->projection_mat * _map->view_mat;
}

const glm::vec4 CameraController::get_position()
{
    return _map->position;
}

void CameraController::set_position(glm::vec4 position)
{
    _map->position = position;
    _map->view_mat = glm::lookAt(glm::vec3(_map->position), glm::vec3(_map->position + _map->direction), glm::vec3(_map->top));
    _map->result_mat = _map->projection_mat * _map->view_mat;
}

const glm::vec4 CameraController::get_direction()
{
    return _map->direction;
}

void CameraController::set_direction(glm::vec4 direction)
{
    _map->direction = glm::normalize(direction);
    _map->view_mat = glm::lookAt(glm::vec3(_map->position), glm::vec3(_map->position + _map->direction), glm::vec3(_map->top));
    _map->result_mat = _map->projection_mat * _map->view_mat;
}

double CameraController::get_sensivity() const
{
    return _cursor_sensivity;
}

void CameraController::set_sensivity(double sensivity)
{
    _cursor_sensivity = sensivity;
}

void CameraController::capture(GlfwConveyor& conveyor)
{
    double prev_x = get_width() / 2;
    double prev_y = get_height() / 2;
    auto cursor_pos_cb = [this, prev_x, prev_y](GlfwConveyor* conveyor, double pos_x, double pos_y) mutable {
        float offset_yaw = _cursor_sensivity * (pos_x - prev_x);
        float offset_pitch = _cursor_sensivity * (pos_y - prev_y);
        prev_x = pos_x;
        prev_y = pos_y;

        auto top = glm::vec3(_map->top);
        auto dir = glm::vec3(_map->direction);
        auto right = glm::normalize(glm::cross(dir, top));
        dir = glm::rotate(dir, offset_yaw, -top);
        dir = glm::rotate(dir, offset_pitch, -right);
        set_direction(glm::vec4(glm::normalize(dir), 0));
    };
    conveyor.cursor_pos_callbacks.emplace_back(std::move(cursor_pos_cb));

    float speed = 0.1;
    auto key_cb = [this, speed](GlfwConveyor* conveyor, int key, int scancode, int action, int mods){
        if (action == GLFW_PRESS || action == GLFW_REPEAT){
            auto pos = glm::vec3(_map->position);
            auto top = glm::vec3(_map->top);
            auto dir = glm::vec3(_map->direction);
            auto right = glm::normalize(glm::cross(dir, top));

            switch (key){
            case GLFW_KEY_W: pos = pos + speed * dir; break;
            case GLFW_KEY_A: pos = pos - speed * right; break;
            case GLFW_KEY_S: pos = pos - speed * dir; break;
            case GLFW_KEY_D: pos = pos + speed * right; break;
            case GLFW_KEY_LEFT_CONTROL: pos = pos - speed * top; break;
            case GLFW_KEY_SPACE: pos = pos + speed * top; break;
            }

            set_position(glm::vec4(pos, 0));
        }
    };
    conveyor.key_callbacks.emplace_back(std::move(key_cb));
}