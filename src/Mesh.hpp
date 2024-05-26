#pragma once

#include <memory>
#include <variant>

#include "Ctx.hpp"
#include "gl/container/IBufferContainer.hpp"

namespace gnev {

class Mesh {
public:
    static unsigned getMaxAttributes();

private:
    std::shared_ptr<IBufferContainer> indices;
    
    struct AttributeInfo {
        std::variant<GLuint, std::string> position;
        GLint elements;
        GLenum type;
        GLboolean normalized;
        GLuint relative_offset;
        std::shared_ptr<IBufferContainer> buffer;
    };
    std::vector<std::unique_ptr<AttributeInfo>> attributes;

};

};