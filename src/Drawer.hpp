#pragma once

#include "glad/gl.h"

#include "view/Camera.hpp"
// #include "texture/TextureChunk.hpp"

#include "gl/GLBuffer.hpp"
#include "gl/GLProgram.hpp"
#include "gl/GLSampler.hpp"
#include "gl/GLShader.hpp"
#include "gl/GLTexture.hpp"
#include "gl/GLVertexArray.hpp"

namespace gnev {

class EXPORT Drawer final {
public:
    Drawer(GLADloadfunc load_func);
    ~Drawer();

    const std::shared_ptr<GladGLContext> ctx;
    GLProgram program;
    Camera camera;

    void draw() const;

private:
    static std::shared_ptr<GladGLContext> create_glad_ctx(GLADloadfunc load_func);


    static std::unique_ptr<GLSampler> create_texture_sampler(const std::shared_ptr<GladGLContext> &ctx);
};

}