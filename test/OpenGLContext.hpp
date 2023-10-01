#pragma once

#include <memory>

#include "gl/Ctx.hpp"
#include "util/Util.hpp"

struct GLFWwindow;

class EXPORT OpenGLContext {
public:
    OpenGLContext();
    virtual ~OpenGLContext();

    const gnev::gl::Ctx& getCtx() const;

private:
    using Window = std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)>;

    Window window;
    gnev::gl::Ctx ctx;

    Window createWindow();
    static void destroyWindow(GLFWwindow* window);
};