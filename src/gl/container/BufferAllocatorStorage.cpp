#include "gl/container/BufferAllocatorStorage.hpp"

#include <memory>

#include "gl/Buffer.hpp"

namespace gnev::gl {

BufferAllocatorStorage::BufferAllocatorStorage(BufferStorageFlags flags)
    : flags{flags} {}

std::unique_ptr<gl::Buffer> BufferAllocatorStorage::allocate(unsigned size,
                                                             const void* initial) const {
    auto buffer = std::make_unique<gl::Buffer>();
    buffer->initStorage(size, initial, flags);
    return buffer;
}

} // namespace gnev::gl