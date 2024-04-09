#include "GlfwWindow.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "OpenGLContext.hpp"

GlfwWindow::GlfwWindow(unsigned int width, unsigned int height, quill::Logger* logger)
    : OpenGLContext(width, height, true, logger) {
    glfwSetWindowUserPointer(&getWindow(), this);

    glfwSetKeyCallback(&getWindow(), glfwKeyCallback);
}

GlfwWindow::~GlfwWindow() {}

void GlfwWindow::pollEvents() { glfwPollEvents(); }

void GlfwWindow::swapBuffers() { glfwSwapBuffers(&getWindow()); }

void GlfwWindow::setKeyCB(const std::optional<KeyCB>& cb) { key_cb = cb; }

void GlfwWindow::glfwKeyCallback(GLFWwindow* window,
                                 int key,
                                 int scancode,
                                 int action,
                                 int mods) {
    auto wnd = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
    if (wnd and wnd->key_cb.has_value()) {
        wnd->key_cb.value()(*wnd, key, scancode, action, mods);
    }
};