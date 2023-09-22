#include "GlfwConveyor.hpp"

#include <future>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

std::atomic<unsigned int>& glfw_windows_counter() {
    static std::atomic<unsigned int> counter(0);
    return counter;
}

GlfwConveyor::GlfwConveyor(int width, int height)
    : window(create_glfw_window(width, height)) {
    glfwSetWindowUserPointer(window.get(), this);

    _previous_key_callback = glfwSetKeyCallback(window.get(), glfw_key_callback);
    _previous_cursor_pos_callback =
        glfwSetCursorPosCallback(window.get(), glfw_cursor_pos_callback);
}

GlfwConveyor::~GlfwConveyor() {}

GlfwConveyor::GLFWGetProcAddress GlfwConveyor::get_proc_address() const {
    return glfwGetProcAddress;
}

void GlfwConveyor::poll_events() const { glfwPollEvents(); }

void GlfwConveyor::swap_buffers() const { glfwSwapBuffers(window.get()); }

std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)>
GlfwConveyor::create_glfw_window(int width, int height) {
    if (glfw_windows_counter().fetch_add(1) == 0 && !glfwInit()) {
        return {nullptr, &destroy_glfw_window};
    }

    std::vector<std::pair<int, int>> hints = {{GLFW_CONTEXT_VERSION_MAJOR, 4},
                                              {GLFW_CONTEXT_VERSION_MINOR, 6},
                                              // {GLFW_REFRESH_RATE, 60},
                                              {GLFW_OPENGL_DEBUG_CONTEXT, true}};

    for (auto& hint : hints) {
        glfwWindowHint(hint.first, hint.second);
    }

    auto monitor = glfwGetPrimaryMonitor();
    auto mode = glfwGetVideoMode(monitor);
    auto window = glfwCreateWindow(mode->width, mode->height, "GLFW", NULL, NULL);
    glfwMakeContextCurrent(window);
    // glfwSwapInterval(0); // Disable vsync

    return {window, &destroy_glfw_window};
}

void GlfwConveyor::destroy_glfw_window(GLFWwindow* window) {
    if (!window) {
        return;
    }

    glfwDestroyWindow(window);
    if (glfw_windows_counter().fetch_sub(1) == 1) {
        glfwTerminate();
    }
}

void GlfwConveyor::glfw_key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods) {
    GlfwConveyor* conveyor = static_cast<GlfwConveyor*>(glfwGetWindowUserPointer(window));

    if (!conveyor) {
        return;
    }

    if (conveyor->_previous_key_callback) {
        conveyor->_previous_key_callback(window, key, scancode, action, mods);
    }

    for (auto& callback : conveyor->key_callbacks) {
        callback(conveyor, key, scancode, action, mods);
    }
}

void GlfwConveyor::glfw_cursor_pos_callback(GLFWwindow* window, double pos_x, double pos_y) {
    GlfwConveyor* conveyor = static_cast<GlfwConveyor*>(glfwGetWindowUserPointer(window));

    if (!conveyor) {
        return;
    }

    if (conveyor->_previous_cursor_pos_callback) {
        conveyor->_previous_cursor_pos_callback(window, pos_x, pos_y);
    }

    for (auto& callback : conveyor->cursor_pos_callbacks) {
        callback(conveyor, pos_x, pos_y);
    }
}