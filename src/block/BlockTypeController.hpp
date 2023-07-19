#pragma once

#include <filesystem>
#include <memory>

#include "gl/GLBufferVector.hpp"
#include "texture/TextureController.hpp"
#include "util/Util.hpp"

namespace gnev {

enum class BlockSide : GLuint {
    Top = 0,
    Bottom = 1,
    Front = 2,
    Left = 3,
    Right = 4,
    Back = 5
};

class EXPORT BlockTypeController final {
    struct BlockTypeData {
        GLint texture[6]; 
    };

public:
    static constexpr GLsizei TEX_WIDTH = 16;
    static constexpr GLsizei TEX_HEIGHT = 16;
    static const std::unordered_map<std::string, BlockSide> SIDE_MAP;

    BlockTypeController(const std::shared_ptr<GladGLContext>& ctx);
    ~BlockTypeController();

    GLint add(const std::filesystem::path& path);

    GLBufferVector<BlockTypeData>& types_buffer();
    const GLBufferVector<BlockTypeData>& types_buffer() const;
    const TextureController& textures_controller() const;

private:
    GLint id = 0;

    GLBufferVector<BlockTypeData> _types_buffer;
    TextureController _textures_controller;

};

}