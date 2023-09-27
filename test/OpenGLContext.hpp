#pragma once

#include <memory>

#include "glad/gl.h"
#include "util/Util.hpp"

struct GLFWwindow;

class EXPORT OpenGLContext {
public:
    using Ctx = std::shared_ptr<GladGLContext>;

    OpenGLContext();
    virtual ~OpenGLContext();

    Ctx getCtx() const;

private:
    using Window = std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)>;

    Window window;
    Ctx ctx;

    Window createWindow();
    static void destroyWindow(GLFWwindow* window);
    static Ctx createCtx();
};