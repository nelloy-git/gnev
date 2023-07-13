#pragma once

#include <filesystem>
#include <memory>

#include "gl/GLVectorBuffer.hpp"
#include "util/Util.hpp"

namespace gnev {

class EXPORT BlockTypeController final {
public:
    BlockTypeController(const std::shared_ptr<GladGLContext>& ctx);
    ~BlockTypeController();

    GLint add(const std::filesystem::path& path);

    GLint handle() const;

private:
    struct BlockTypeData {
        GLint texture[6]; 
    };

    GLVectorBuffer<BlockTypeData> _buffer;


};

}