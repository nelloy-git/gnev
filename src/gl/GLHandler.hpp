#pragma once

#include <memory>

#include "glad/gl.h"

#include "util/Util.hpp"

namespace gnev {

class EXPORT GLHandler {
public:
    using Deleter = void(*)(GLuint* handle, GladGLContext& ctx);

    GLHandler(const std::shared_ptr<GladGLContext>& ctx, GLuint* handle, const Deleter& deleter);
    virtual ~GLHandler();

    virtual const std::shared_ptr<GladGLContext>& ctx() const;
    virtual GLuint handle() const;

private:
    std::shared_ptr<GladGLContext> _ctx;
    std::shared_ptr<const GLuint> _handle;

};

}