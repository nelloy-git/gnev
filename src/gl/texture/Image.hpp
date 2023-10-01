#pragma once

#include "gl/Texture.hpp"

namespace gnev::gl::texture {

struct Image {
    std::size_t level;
    std::size_t x;
    std::size_t y;
    std::size_t width;
    std::size_t height;
    GLenum format;
    GLenum type;
    std::shared_ptr<void> data;
};

} // namespace gnev::gl::texture