#pragma once

#include <memory>

#include "utils/Worker.hpp"

struct GLFWwindow;

class __declspec(dllexport) GlfwConveyor {
public:
    GlfwConveyor();
    virtual ~GlfwConveyor();

    typedef void (*GLFWglproc)(void);
    typedef GLFWglproc (*GLFWGetProcAddress)(const char *procname);

    GLFWGetProcAddress get_proc_address() const;
    void poll_events() const;
    void swap_buffers() const;

    const Worker worker;
    GLFWwindow* const window;

    std::vector<std::function<void(GlfwConveyor* conveyor, int key, int scancode, int action, int mods)>> key_callbacks;
    std::vector<std::function<void(GlfwConveyor* conveyor, double pos_x, double pos_y)>> cursor_pos_callbacks;

private:
    typedef void (*GlfwKeyCallback)(GLFWwindow* window, int key, int scancode, int action, int mods);
    GlfwKeyCallback _previous_key_callback;

    typedef void (*GlfwCursorPosCallback)(GLFWwindow* window, double pos_x, double pos_y);
    GlfwCursorPosCallback _previous_cursor_pos_callback;
    
    // std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> _window;

    static GLFWwindow* create_glfw_window(const Worker& worker);
    static void destroy_glfw_window(GLFWwindow* window);

    static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void glfw_cursor_pos_callback(GLFWwindow* window, double pos_x, double pos_y);
};