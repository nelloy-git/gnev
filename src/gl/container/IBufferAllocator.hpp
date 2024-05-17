#pragma once

#include <memory>

#include "gl/Buffer.hpp"

namespace gnev {

class IBufferAllocator {
public:
    virtual ~IBufferAllocator() = default;

    virtual std::unique_ptr<gl::Buffer> allocate(unsigned size,
                                                 const void* initial) const = 0;
};

} // namespace gnev