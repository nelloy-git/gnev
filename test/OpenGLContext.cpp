#include "OpenGLContext.hpp"

#include <stdexcept>
#include <vector>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "gl/Debug.hpp"

OpenGLContext::OpenGLContext()
    : window(createWindow())
    , ctx(glfwGetProcAddress) {

    GLint context_flags;
    ctx.glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
    if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        ctx.glEnable(GL_DEBUG_OUTPUT);
        ctx.glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        ctx.glDebugMessageCallback(&gnev::glDebugOutput, nullptr);
        ctx.glDebugMessageControl(GL_DONT_CARE,
                                  GL_DEBUG_TYPE_ERROR,
                                  GL_DONT_CARE,
                                  0,
                                  nullptr,
                                  GL_TRUE);
        ctx.glDebugMessageControl(GL_DONT_CARE,
                                  GL_DEBUG_TYPE_PERFORMANCE,
                                  GL_DONT_CARE,
                                  0,
                                  nullptr,
                                  GL_FALSE);
    }
}

OpenGLContext::~OpenGLContext() {}

const gnev::gl::Ctx& OpenGLContext::getCtx() const { return ctx; }

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