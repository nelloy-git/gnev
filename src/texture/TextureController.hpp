#pragma once

#include <filesystem>
#include <vector>
#include <unordered_map>

#include "texture/TextureChunk.hpp"
#include "util/Util.hpp"

namespace gnev {

class EXPORT TextureController final {
public:
    TextureController(const std::shared_ptr<GladGLContext>& ctx);
    ~TextureController();

    bool init(GLsizei width, GLsizei height);
    GLint load(const std::filesystem::path& path);

private:
    const std::shared_ptr<GladGLContext> _ctx;
    std::vector<TextureChunk> _chunks;

    std::unordered_map<std::wstring, GLint> _path_map;

};

}