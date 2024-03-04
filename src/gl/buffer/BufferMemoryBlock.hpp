#pragma once

namespace gnev::gl {

class IBufferAllocator;

struct BufferMemoryBlock {
public:
    BufferMemoryBlock(IBufferAllocator& allocator, unsigned offset, unsigned size)
        : allocator{allocator}
        , offset{offset}
        , size{size} {}

    IBufferAllocator& getAllocator() const { return allocator; }

    unsigned getOffset() const { return offset; }

    unsigned getSize() const { return size; }

private:
    IBufferAllocator& allocator;
    unsigned offset;
    unsigned size;
};

} // namespace gnev::gl