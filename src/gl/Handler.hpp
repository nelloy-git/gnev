#pragma once

#include <memory>

#include "glad/gl.h"

#include "util/Util.hpp"

namespace gnev::gl {

using GladCtx = std::shared_ptr<GladGLContext>;

class EXPORT Handler {
public:
    using Deleter = void(*)(GLuint* handle, GladGLContext& ctx);

    Handler(const GladCtx& ctx, GLuint* handle, const Deleter& deleter);
    virtual ~Handler();

    virtual const GladCtx& ctx() const;
    virtual GLuint handle() const;

private:
    GladCtx _ctx;
    std::shared_ptr<const GLuint> _handle;

};

}