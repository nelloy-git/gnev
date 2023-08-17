#pragma once

#include "glad/gl.h"

#include "gl/Program.hpp"
#include "gl/Sampler.hpp"

#include "view/Camera.hpp"

namespace gnev {

class EXPORT Drawer final {
public:
    Drawer(GLADloadfunc load_func);
    ~Drawer();

    const std::shared_ptr<GladGLContext> ctx;
    gl::Program program;
    Camera camera;
    gl::Sampler sampler;

    void draw() const;

private:

    static std::shared_ptr<GladGLContext> create_glad_ctx(GLADloadfunc load_func);
    static gl::Sampler create_texture_sampler(const std::shared_ptr<GladGLContext> &ctx);
};

}