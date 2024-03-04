#include "gl/buffer/BufferAllocatorFragmented.hpp"

namespace gnev::gl {

BufferAllocatorFragmented::BufferAllocatorFragmented(std::unique_ptr<Buffer>&& buffer_)
    : IBufferAllocator{std::move(buffer_)} {
    auto size = buffer->getSize();
    if (size < 0) {
        throw std::logic_error("Buffer has negative size");
    }
    free_blocks.push_back({.offset = 0, .size = static_cast<unsigned>(size)});
}

BufferMemoryBlock* BufferAllocatorFragmented::allocate(unsigned size) { return nullptr; }

void BufferAllocatorFragmented::deallocate(const BufferMemoryBlock*) {}

} // namespace gnev::gl