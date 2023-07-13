#include "GlfwConveyor.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <iostream>
#include <future>

std::atomic<unsigned int>& glfw_windows_counter(){
    static std::atomic<unsigned int> counter(0);
    return counter;
}

GlfwConveyor::GlfwConveyor() :
    worker(), 
    window(create_glfw_window(worker)){
    glfwSetWindowUserPointer(window, this);

    _previous_key_callback = glfwSetKeyCallback(window, glfw_key_callback);
}

GlfwConveyor::~GlfwConveyor(){
    worker.push([window = window](){
        destroy_glfw_window(window);
    });
}

GlfwConveyor::GLFWGetProcAddress GlfwConveyor::get_proc_address() const {
    return glfwGetProcAddress;
}

void GlfwConveyor::poll_events() const {
    glfwPollEvents();
}

void GlfwConveyor::swap_buffers() const {
    glfwSwapBuffers(window);
}

GLFWwindow* GlfwConveyor::create_glfw_window(const Worker& worker){
    auto promise = std::make_shared<std::promise<GLFWwindow*>>();
    auto future = promise->get_future();

    worker.push([promise](){
        if (glfw_windows_counter().fetch_add(1) == 0 && !glfwInit()){
            promise->set_value(nullptr);
            return;
        }

        std::vector<std::pair<int, int>> hints = {
            {GLFW_CONTEXT_VERSION_MAJOR, 4},
            {GLFW_CONTEXT_VERSION_MINOR, 6},
            // {GLFW_REFRESH_RATE, 60},
            {GLFW_OPENGL_DEBUG_CONTEXT, true}
        };

        for (auto& hint : hints){
            glfwWindowHint(hint.first, hint.second);
        }

        auto window = glfwCreateWindow(1920, 1080, "GLFW", NULL, NULL);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0); // Disable vsync

        promise->set_value(window);
    });

    return future.get();
}

void GlfwConveyor::destroy_glfw_window(GLFWwindow* window){
    if (!window){
        return;
    }

    glfwDestroyWindow(window);
    if (glfw_windows_counter().fetch_sub(1) == 1){
        glfwTerminate();
    }
}

void GlfwConveyor::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    GlfwConveyor* conveyor = static_cast<GlfwConveyor*>(glfwGetWindowUserPointer(window));

    if (!conveyor){
        return;
    }

    if (conveyor->_previous_key_callback){
        conveyor->_previous_key_callback(window, key, scancode, action, mods);
    }

    if (conveyor->key_callback){
        conveyor->key_callback(conveyor, key, scancode, action, mods);
    }
}