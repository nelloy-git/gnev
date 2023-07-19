#pragma once

#include <string>

#include "gl/GLTexture.hpp"
#include "util/Util.hpp"

namespace gnev {

class EXPORT TextureChunk final {
public:
    static constexpr GLsizei CAPACITY_BASE = 1;
    static constexpr GLuint CAPACITY_MULT = 2;
    static GLint MAX_CAPACITY(GladGLContext& ctx);

    TextureChunk(const std::shared_ptr<GladGLContext>& ctx, GLsizei width, GLsizei height);
    ~TextureChunk(); 

    bool expand(GLsizei capacity);
    GLint add(const void* data);

    GLTexture& texture();
    const GLTexture& texture() const;

    const std::shared_ptr<GladGLContext>& ctx() const;
    GLsizei width() const;
    GLsizei height() const;
    GLsizei size() const;
    GLsizei capacity() const;

private:
    GLTexture _texture_array;

    GLsizei _width = 0;
    GLsizei _height = 0;
    GLsizei _size = 0;
    GLsizei _capacity = CAPACITY_BASE;

};

}