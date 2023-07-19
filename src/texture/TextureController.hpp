#pragma once

#include <filesystem>
#include <list>
#include <unordered_map>

#include "texture/TextureChunk.hpp"
#include "util/Util.hpp"

namespace gnev {

class EXPORT TextureController final {
public:
    TextureController(const std::shared_ptr<GladGLContext>& ctx, GLsizei img_width, GLsizei img_height);
    ~TextureController();

    GLuint bind(GLenum first_target = GL_TEXTURE0) const;
    GLint load(const std::filesystem::path& path);

    const std::shared_ptr<GladGLContext>& ctx() const;

private:
    std::vector<TextureChunk> _chunks;

    std::unordered_map<std::string, GLint> _path_map;

    static TextureChunk create_chunk(const std::shared_ptr<GladGLContext>& ctx, GLsizei width, GLsizei height);

};

}