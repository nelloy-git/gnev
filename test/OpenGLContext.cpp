#include "OpenGLContext.hpp"

#include <stdexcept>
#include <vector>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

OpenGLContext::OpenGLContext()
    : window(createWindow())
    , ctx(createCtx()) {}

OpenGLContext::~OpenGLContext() {}

OpenGLContext::Ctx OpenGLContext::getCtx() const { return ctx; }

OpenGLContext::Window OpenGLContext::createWindow() {
    if (!glfwInit()) {
        throw std::runtime_error("Can not init GLFW");
    }

    auto monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        throw std::runtime_error("Can not get primary monitor");
    }

    auto mode = glfwGetVideoMode(monitor);
    if (!mode) {
        throw std::runtime_error("Can not get video mode");
    }

    std::vector<std::pair<int, int>> hints = {{GLFW_CONTEXT_VERSION_MAJOR, 4},
                                              {GLFW_CONTEXT_VERSION_MINOR, 6},
                                              {GLFW_REFRESH_RATE, 60},
                                              {GLFW_OPENGL_DEBUG_CONTEXT, true},
                                              {GLFW_VISIBLE, false}};

    for (auto& hint : hints) {
        glfwWindowHint(hint.first, hint.second);
    }
    auto window = glfwCreateWindow(64, 64, "OpenGLContext", NULL, NULL);
    glfwMakeContextCurrent(window);

    return {window, &destroyWindow};
}

void OpenGLContext::destroyWindow(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

OpenGLContext::Ctx OpenGLContext::createCtx() {
    auto ctx = std::make_shared<GladGLContext>();
    gladLoadGLContext(ctx.get(), glfwGetProcAddress);
    return ctx;
}