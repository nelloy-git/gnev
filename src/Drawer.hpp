#pragma once

#include "glad/gl.h"

#include "view/Camera.hpp"
#include "texture/TextureChunk.hpp"

#include "gl/GLBuffer.hpp"
#include "gl/GLProgram.hpp"
#include "gl/GLSampler.hpp"
#include "gl/GLShader.hpp"
#include "gl/GLTexture.hpp"
#include "gl/GLVertexArray.hpp"

namespace gnev {

class __declspec(dllexport) Drawer final {
public:
    Drawer();
    ~Drawer();

    std::pair<bool, std::string> init(GLADloadfunc load_func);
    void add_voxel(float x, float y, float z);

    std::unique_ptr<Camera> camera;

    void draw() const;

private:
    std::shared_ptr<GladGLContext> ctx;

    std::unique_ptr<GLVertexArray> _voxel_vao;
    std::unique_ptr<GLBuffer> _voxel_buffer;
    std::unique_ptr<GLTexture> _voxel_texture_array;
    std::unique_ptr<TextureChunk> _texture_chunk;
    std::unique_ptr<GLSampler> _texture_sampler;

    std::unique_ptr<GLShader> _vertex_shader;
    std::unique_ptr<GLShader> _geometry_shader;
    std::unique_ptr<GLShader> _fragment_shader;
    std::unique_ptr<GLProgram> _program;

    GLuint _capacity = 1'000'000'000;
    GLuint _count = 0;

    static GladGLContext create_glad_ctx(GLADloadfunc load_func);

    static std::unique_ptr<TextureChunk> create_texture_chunk(const std::shared_ptr<GladGLContext> &ctx);
    static std::unique_ptr<GLSampler> create_texture_sampler(const std::shared_ptr<GladGLContext> &ctx);
};

}