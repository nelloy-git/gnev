#pragma once

#include "Ctx.hpp"
#include "gl/container/IBufferAccessor.hpp"
#include "gl/enum/BufferMapRangeAccess.hpp"

namespace gnev::gl {

class EXPORT BufferAccessorMapped : public IBufferAccessor {
public:
    BufferAccessorMapped(BufferMapRangeAccess access);

    bool set(Buffer& buffer, unsigned offset, unsigned size, const void* data) override;
    bool get(Buffer& buffer, unsigned offset, unsigned size, void* data) override;
    bool change(Buffer& buffer,
                unsigned offset,
                unsigned size,
                const Changer& changer) override;
    bool copy(Buffer& buffer,
              unsigned src_offset,
              unsigned dst_offset,
              unsigned size) override;

private:
    BufferMapRangeAccess access;
    GLuint current_buffer = GL_INVALID_INDEX;
    GLbyte* map = nullptr;

    void checkMap(Buffer& buffer);
};

} // namespace gnev::gl