#pragma once

#include "gl/buffer/IBufferAccessor.hpp"

namespace gnev::gl {

class EXPORT BufferRawAccessorMappedCoherent : public IBufferAccessor {
public:
    BufferRawAccessorMappedCoherent(Buffer& buffer);
    ~BufferRawAccessorMappedCoherent();

    void set(GLintptr offset, GLintptr size, const void* data) override;
    void get(GLintptr offset, GLintptr size, void* data) override;
    void change(GLintptr offset, GLintptr size, const Changer& changer) override;
    void copy(GLintptr src_offset, GLintptr dst_offset, GLintptr size) override;

private:
    GLbyte* map;
};

} // namespace gnev::gl