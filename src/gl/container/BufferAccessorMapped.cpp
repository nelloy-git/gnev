#include "gl/container/BufferAccessorMapped.hpp"

#include "Ctx.hpp"
#include "gl/Buffer.hpp"

namespace gnev::gl {

BufferAccessorMapped::BufferAccessorMapped(BufferMapRangeAccess access)
    : access{access} {}

bool BufferAccessorMapped::set(Buffer& buffer,
                               unsigned offset,
                               unsigned size,
                               const void* src) {
    GNEV_BUFFER_ACCESSOR_LOG(L2, fmtquill::to_string(buffer), offset, size, src);
    checkMap(buffer);
    std::memcpy(map + offset, src, size);
    return true;
}

bool BufferAccessorMapped::get(Buffer& buffer,
                               unsigned offset,
                               unsigned size,
                               void* dst) {
    GNEV_BUFFER_ACCESSOR_LOG(L2, fmtquill::to_string(buffer), offset, size, dst);
    checkMap(buffer);
    std::memcpy(dst, map + offset, size);
    return true;
}

bool BufferAccessorMapped::change(Buffer& buffer,
                                  unsigned offset,
                                  unsigned size,
                                  const Changer& changer) {
    GNEV_BUFFER_ACCESSOR_LOG(L2,
                             fmtquill::to_string(buffer),
                             offset,
                             size,
                             (void*)&changer);
    checkMap(buffer);
    changer(map + offset, size);
    return true;
}

bool BufferAccessorMapped::copy(Buffer& buffer,
                                unsigned src_offset,
                                unsigned dst_offset,
                                unsigned size) {
    GNEV_BUFFER_ACCESSOR_LOG(L2,
                             fmtquill::to_string(buffer),
                             src_offset,
                             dst_offset,
                             size);
    checkMap(buffer);
    std::memcpy(map + dst_offset, map + src_offset, size);
    return true;
}

void BufferAccessorMapped::checkMap(Buffer& buffer) {
    if (buffer.handle() != current_buffer) {
        GNEV_LOG_L2("\tremap from Buffer#{} to {}",
                    current_buffer,
                    fmtquill::to_string(buffer));
        current_buffer = buffer.handle();
        map = static_cast<GLbyte*>(buffer.mapRange(0, buffer.getSize(), access));
    }
}

} // namespace gnev::gl