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
    : _camera(ctx, &default_camera, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT)
{
    auto map = _camera.glMapBuffer(GL_READ_WRITE);
    map->position = glm::vec4{1, 1, 1, 0};
    map->direction = glm::normalize(glm::vec4{-1, -1, -1, 0});
    map->view_mat = glm::lookAt(glm::vec3{1, 1, 1}, {0, 0, 0}, {0, 1, 0});
    map->projection_mat = glm::perspectiveFov(map->fov, static_cast<float>(map->width), static_cast<float>(map->height), map->near_z, map->far_z);
    map->result_mat = map->projection_mat * map->view_mat;
    _camera.glUnmapBuffer();
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
    auto map = _camera.glMapBuffer(GL_READ_ONLY);
    auto fov = map->fov;
    _camera.glUnmapBuffer();
    return fov;
}

void CameraController::set_fov(GLfloat fov)
{
    auto map = _camera.glMapBuffer(GL_READ_WRITE);
    map->fov = fov;
    map->projection_mat = glm::perspectiveFov(map->fov, static_cast<float>(map->width), static_cast<float>(map->height), map->near_z, map->far_z);
    map->result_mat = map->projection_mat * map->view_mat;
    _camera.glUnmapBuffer();
}

GLfloat CameraController::get_width()
{
    auto map = _camera.glMapBuffer(GL_READ_ONLY);
    auto width = map->width;
    _camera.glUnmapBuffer();
    return width;
}

void CameraController::set_width(GLfloat width)
{
    auto map = _camera.glMapBuffer(GL_READ_WRITE);
    map->width = width;
    map->projection_mat = glm::perspectiveFov(map->fov, static_cast<float>(map->width), static_cast<float>(map->height), map->near_z, map->far_z);
    map->result_mat = map->projection_mat * map->view_mat;
    _camera.glUnmapBuffer();
}

GLfloat CameraController::get_height()
{
    auto map = _camera.glMapBuffer(GL_READ_ONLY);
    auto height = map->height;
    _camera.glUnmapBuffer();
    return height;
}

void CameraController::set_height(GLfloat height)
{
    auto map = _camera.glMapBuffer(GL_READ_WRITE);
    map->height = height;
    map->projection_mat = glm::perspectiveFov(map->fov, static_cast<float>(map->width), static_cast<float>(map->height), map->near_z, map->far_z);
    map->result_mat = map->projection_mat * map->view_mat;
    _camera.glUnmapBuffer();
}

GLfloat CameraController::get_near_z()
{
    auto map = _camera.glMapBuffer(GL_READ_ONLY);
    auto near_z = map->near_z;
    _camera.glUnmapBuffer();
    return near_z;
}

void CameraController::set_near_z(GLfloat near_z)
{
    auto map = _camera.glMapBuffer(GL_READ_WRITE);
    map->near_z = near_z;
    map->projection_mat = glm::perspectiveFov(map->fov, static_cast<float>(map->width), static_cast<float>(map->height), map->near_z, map->far_z);
    map->result_mat = map->projection_mat * map->view_mat;
    _camera.glUnmapBuffer();
}

GLfloat CameraController::get_far_z()
{
    auto map = _camera.glMapBuffer(GL_READ_ONLY);
    auto far_z = map->far_z;
    _camera.glUnmapBuffer();
    return far_z;
}

void CameraController::set_far_z(GLfloat far_z)
{
    auto map = _camera.glMapBuffer(GL_READ_WRITE);
    map->far_z = far_z;
    map->projection_mat = glm::perspectiveFov(map->fov, static_cast<float>(map->width), static_cast<float>(map->height), map->near_z, map->far_z);
    map->result_mat = map->projection_mat * map->view_mat;
    _camera.glUnmapBuffer();
}

const glm::vec4 CameraController::get_position()
{
    auto map = _camera.glMapBuffer(GL_READ_ONLY);
    auto position = map->position;
    _camera.glUnmapBuffer();
    return position;
}

void CameraController::set_position(glm::vec4 position)
{
    auto map = _camera.glMapBuffer(GL_READ_WRITE);
    map->position = position;
    map->view_mat = glm::lookAt(glm::vec3(map->position), glm::vec3(map->position + map->direction), glm::vec3(map->top));
    map->result_mat = map->projection_mat * map->view_mat;
    _camera.glUnmapBuffer();
}

const glm::vec4 CameraController::get_direction()
{
    auto map = _camera.glMapBuffer(GL_READ_ONLY);
    auto direction = map->direction;
    _camera.glUnmapBuffer();
    return direction;
}

void CameraController::set_direction(glm::vec4 direction)
{
    auto map = _camera.glMapBuffer(GL_READ_WRITE);
    map->direction = glm::normalize(direction);
    map->view_mat = glm::lookAt(glm::vec3(map->position), glm::vec3(map->position + map->direction), glm::vec3(map->top));
    map->result_mat = map->projection_mat * map->view_mat;
    _camera.glUnmapBuffer();
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
        std::cout << "mouse moved" << std::endl;
        float offset_yaw = _cursor_sensivity * (pos_x - prev_x);
        float offset_pitch = _cursor_sensivity * (pos_y - prev_y);
        prev_x = pos_x;
        prev_y = pos_y;

        auto map = _camera.glMapBuffer(GL_READ_WRITE);
        auto top = glm::vec3(map->top);
        auto dir = glm::vec3(map->direction);
        auto right = glm::normalize(glm::cross(dir, top));
        dir = glm::rotate(dir, offset_yaw, -top);
        dir = glm::rotate(dir, offset_pitch, -right);
        _camera.glUnmapBuffer();
        set_direction(glm::vec4(glm::normalize(dir), 0));
    };
    conveyor.cursor_pos_callbacks.emplace_back(std::move(cursor_pos_cb));

    float speed = 0.1;
    auto key_cb = [this, speed](GlfwConveyor* conveyor, int key, int scancode, int action, int mods){
        std::cout << "Key pressed" << std::endl;
        if (action == GLFW_PRESS || action == GLFW_REPEAT){
            auto map = _camera.glMapBuffer(GL_READ_WRITE);
            auto pos = glm::vec3(map->position);
            auto top = glm::vec3(map->top);
            auto dir = glm::vec3(map->direction);
            auto right = glm::normalize(glm::cross(dir, top));
            _camera.glUnmapBuffer();

            switch (key){
            case GLFW_KEY_W: pos = pos + speed * dir; break;
            case GLFW_KEY_A: pos = pos - speed * right; break;
            case GLFW_KEY_S: pos = pos - speed * dir; break;
            case GLFW_KEY_D: pos = pos + speed * right; break;
            case GLFW_KEY_LEFT_CONTROL: pos = pos - speed * top; break;
            case GLFW_KEY_SPACE: pos = pos + speed * top; break;
            }
            std::cout << "{" << pos.x << ", " << pos.y << ", " << pos.z << "}" << std::endl;

            set_position(glm::vec4(pos, 0));
        }
    };
    conveyor.key_callbacks.emplace_back(std::move(key_cb));
}