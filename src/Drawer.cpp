#include "Drawer.hpp"

#include <array>
#include <iostream>
#include <stdexcept>

#include "gl/Debug.hpp"
#include "glm/glm.hpp"
#include "stb_image.h"

using namespace gnev;

Drawer::Drawer(GLADloadfunc load_func)
    : ctx(create_glad_ctx(load_func)),
      camera(ctx),
      program(ctx),
      sampler(create_texture_sampler(ctx)) {
    gladLoadGLContext(ctx.get(), load_func);

    // camera.pos = {1.5, 1.5, 1.5};
    // camera.applyLookAt({0, 0, 0});

    // Enable debug
    GLint context_flags;
    ctx->GetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
    if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        std::cout << "glDebugOutput enabled" << std::endl;
        ctx->Enable(GL_DEBUG_OUTPUT);
        ctx->Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        ctx->DebugMessageCallback(glDebugOutput, nullptr);
        ctx->DebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    ctx->ClearColor(0.1, 0.8, 0.1, 1);

    ctx->Enable(GL_CULL_FACE);
    ctx->Enable(GL_DEPTH_TEST);
}

Drawer::~Drawer() {}

void Drawer::draw() const {
    ctx->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.glUseProgram();
    // auto camera_index = program.glGetUniformBlockIndex("Camera");
    // program.glUniformBlockBinding(camera_index, 0);
    // camera.buffer.glBindBufferBase(GL_UNIFORM_BUFFER, 0);
}

std::shared_ptr<GladGLContext> Drawer::create_glad_ctx(GLADloadfunc load_func) {
    auto ctx = std::make_shared<GladGLContext>();
    gladLoadGLContext(ctx.get(), load_func);
    return ctx;
}

gl::Sampler Drawer::create_texture_sampler(const std::shared_ptr<GladGLContext>& ctx) {
    static const float BORDER_COLOR[] = {1.0f, 1.0f, 1.0f, 0.0f};

    auto sampler = gl::Sampler(ctx);

    sampler.glSamplerParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
    sampler.glSamplerParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
    sampler.glSamplerParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    sampler.glSamplerParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // sampler.glSamplerParameterfv(GL_TEXTURE_BORDER_COLOR, BORDER_COLOR);

    return sampler;
}
