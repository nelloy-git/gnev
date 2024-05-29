#pragma once

#include <sstream>

#include "glad/gl.h"
#include "util/Log.hpp"

namespace gnev {

inline const char* glGetErrorString(GLenum error) {
    switch (error) {
    case GL_NO_ERROR:
        return "No Error";
    case GL_INVALID_ENUM:
        return "Invalid Enum";
    case GL_INVALID_VALUE:
        return "Invalid Value";
    case GL_INVALID_OPERATION:
        return "Invalid Operation";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "Invalid Framebuffer Operation";
    case GL_OUT_OF_MEMORY:
        return "Out of Memory";
    case GL_STACK_UNDERFLOW:
        return "Stack Underflow";
    case GL_STACK_OVERFLOW:
        return "Stack Overflow";
    case GL_CONTEXT_LOST:
        return "Context Lost";
    default:
        return "Unknown Error";
    }
};

static void glDebugOutput(GLenum source,
                          GLenum type,
                          GLuint id,
                          GLenum severity,
                          GLsizei length,
                          const GLchar* message,
                          const void* userParam) {
    // auto& gl = *static_cast<const GLapi*>(userParam);

    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::stringstream msg;

    msg << "---------------" << std::endl;
    msg << "Debug message " << glGetErrorString(id) << "(" << id << "): " << message
              << std::endl;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        msg << "Source: API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        msg << "Source: Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        msg << "Source: Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        msg << "Source: Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        msg << "Source: Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        msg << "Source: Other";
        break;
    }
    msg << std::endl;

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        msg << "Type: Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        msg << "Type: Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        msg << "Type: Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        msg << "Type: Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        msg << "Type: Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        msg << "Type: Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        msg << "Type: Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        msg << "Type: Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        msg << "Type: Other";
        break;
    }
    msg << std::endl;

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        msg << "Severity: high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        msg << "Severity: medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        msg << "Severity: low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        msg << "Severity: notification";
        break;
    }
    msg << std::endl;

    GNEV_LOG_ERROR("{}", msg.str());
};

} // namespace gnev