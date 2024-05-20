#pragma once

#include "gl/container/IBufferAllocator.hpp"

namespace gnev::gl {

class EXPORT BufferAllocatorStorage : public IBufferAllocator {
public:
    BufferAllocatorStorage(BufferStorageFlags flags);

    virtual std::unique_ptr<gl::Buffer> allocate(unsigned size,
                                                 const void* initial) const override;

private:
    BufferStorageFlags flags;
};

} // namespace gnev::gl