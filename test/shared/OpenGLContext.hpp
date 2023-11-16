#pragma once

#include <memory>

#include "gl/Ctx.hpp"
#include "util/Util.hpp"

struct GLFWwindow;

class EXPORT OpenGLContext {
public:
    OpenGLContext(unsigned int width = WINDOW_SIZE,
                  unsigned int height = WINDOW_SIZE,
                  bool visible = false);
    virtual ~OpenGLContext();

    static const gnev::gl::Ctx& getCtx();
    GLFWwindow& getWindow();

private:
    static constexpr unsigned int WINDOW_SIZE = 64;

    using Window = std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)>;

    Window window;

    Window createWindow(unsigned int width, unsigned int height, bool visible);
    static void destroyWindow(GLFWwindow* window);
};