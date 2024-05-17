#pragma once

#include "Ctx.hpp"

namespace gnev {

class IBufferContainer {
public:
    virtual GLuint handle() const = 0;
};

}