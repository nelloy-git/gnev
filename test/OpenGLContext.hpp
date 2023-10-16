#pragma once

#include <memory>

#include "gl/Ctx.hpp"
#include "util/Util.hpp"

struct GLFWwindow;

class EXPORT OpenGLContext {
public:
    OpenGLContext();
    virtual ~OpenGLContext();

    static const gnev::gl::Ctx& getCtx();

private:
    static constexpr unsigned int WINDOW_SIZE = 64;

    
    using Window = std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)>;

    Window window;

    Window createWindow();
    static void destroyWindow(GLFWwindow* window);
};