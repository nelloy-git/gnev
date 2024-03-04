#pragma once

#include <memory>

#include "gl/Buffer.hpp"
#include "gl/buffer/BufferMemoryBlock.hpp"

namespace gnev::gl {

class IBufferAllocator {
public:
    IBufferAllocator(std::unique_ptr<Buffer>&& buffer_)
        : buffer(std::move(buffer_)){};
    virtual ~IBufferAllocator() = default;

    const Buffer& getBuffer() const { return *buffer; }

    virtual BufferMemoryBlock* allocate(unsigned size) = 0;
    virtual void deallocate(const BufferMemoryBlock*) = 0;

protected:
    std::unique_ptr<Buffer> buffer;
};

} // namespace gnev::gl