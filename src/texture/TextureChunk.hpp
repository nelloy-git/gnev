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

    TextureChunk(const std::shared_ptr<GladGLContext>& ctx);
    ~TextureChunk(); 

    std::pair<bool, std::string> init(GLsizei width, GLsizei height);
    bool expand(GLsizei capacity);
    GLint add(const void* data);

    GLsizei width() const;
    GLsizei height() const;
    GLsizei size() const;
    GLsizei capacity() const;
    GLint handle() const;

private:
    const std::shared_ptr<GladGLContext> _ctx;
    std::unique_ptr<GLTexture> _texture_array;

    GLsizei _width = 0;
    GLsizei _height = 0;
    GLsizei _size = 0;
    GLsizei _capacity = CAPACITY_BASE;

};

}