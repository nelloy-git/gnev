#pragma once

#include <functional>
#include <optional>
#include "OpenGLContext.hpp"

class GlfwWindow : public OpenGLContext {
    using KeyCB = std::function<void(GlfwWindow&, int key, int scancode, int action, int mods)>;

public:
    GlfwWindow(unsigned int width, unsigned int height, quill::Logger* logger = nullptr);
    virtual ~GlfwWindow();

    void pollEvents();
    void swapBuffers();

    void setKeyCB(const std::optional<KeyCB>& cb);

private:
    std::optional<KeyCB> key_cb;

    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};