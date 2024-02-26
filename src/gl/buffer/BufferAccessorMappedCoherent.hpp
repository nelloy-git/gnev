#pragma once

#include "gl/buffer/IBufferAccessor.hpp"

namespace gnev::gl {

class EXPORT BufferAccessorMappedCoherent : public IBufferAccessor {
public:
    BufferAccessorMappedCoherent(std::unique_ptr<Buffer>&& buffer);
    ~BufferAccessorMappedCoherent();

    const Buffer& getBuffer() const override;

    void set(GLintptr offset, GLintptr size, const void* data) override;
    void get(GLintptr offset, GLintptr size, void* data) override;
    void change(GLintptr offset, GLintptr size, const Changer& changer) override;
    void copy(GLintptr src_offset, GLintptr dst_offset, GLintptr size) override;

private:
    std::unique_ptr<Buffer> buffer;
    GLbyte* map;
};

} // namespace gnev::gl