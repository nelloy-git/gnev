#include "gl/container/BufferAccessorSubData.hpp"

#include "gl/Buffer.hpp"
#include "util/Log.hpp"

namespace gnev::gl {

bool BufferAccessorSubData::set(Buffer& buffer,
                                unsigned offset,
                                unsigned size,
                                const void* src) {
    GNEV_BUFFER_ACCESSOR_LOG(L2, fmtquill::to_string(buffer), offset, size, src);
    buffer.setSubData(offset, size, src);
    return true;
}

bool BufferAccessorSubData::get(Buffer& buffer,
                                unsigned offset,
                                unsigned size,
                                void* dst) {
    GNEV_BUFFER_ACCESSOR_LOG(L2, fmtquill::to_string(buffer), offset, size, dst);
    buffer.getSubData(offset, size, dst);
    return true;
}

bool BufferAccessorSubData::change(Buffer& buffer,
                                   unsigned offset,
                                   unsigned size,
                                   const Changer& changer) {
    GNEV_BUFFER_ACCESSOR_LOG(L2,
                             fmtquill::to_string(buffer),
                             offset,
                             size,
                             (void*)&changer);
    void* data = std::malloc(size);
    get(buffer, offset, size, data);
    changer(data, size);
    set(buffer, offset, size, data);
    return true;
}

bool BufferAccessorSubData::copy(Buffer& buffer,
                                 unsigned src_offset,
                                 unsigned dst_offset,
                                 unsigned size) {
    GNEV_BUFFER_ACCESSOR_LOG(L2,
                             fmtquill::to_string(buffer),
                             src_offset,
                             dst_offset,
                             size);
    buffer.copyTo(buffer, src_offset, dst_offset, size);
    return true;
}

} // namespace gnev::gl