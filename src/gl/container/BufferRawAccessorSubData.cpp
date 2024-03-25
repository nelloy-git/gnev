#include "gl/container/BufferRawAccessorSubData.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "util/InstanceLogger.hpp"

namespace gnev::gl {

void BufferRawAccessorSubData::bindBuffer(Buffer* new_buffer) {
    buffer = new_buffer;
    if (buffer->isStorage()) {
        GLbitfield storage_flags = buffer->getStorageFlags();
        if (not(storage_flags & GL_DYNAMIC_STORAGE_BIT)) {
            InstanceLogger{}
                .Log<ERROR,
                     "Buffer<{}> has invalid storage flags {}">(buffer->handle(),
                                                                fmt::BitFlags{
                                                                    storage_flags,
                                                                    fmt::BitFlags::Group::
                                                                        glBufferStorage});
        }
    }
}

Buffer* BufferRawAccessorSubData::getBoundBuffer() const { return buffer; }

bool BufferRawAccessorSubData::set(unsigned offset, unsigned size, const void* src) {
    if (not buffer) {
        return false;
    }
    buffer->setSubData(offset, size, src);
    return true;
}

bool BufferRawAccessorSubData::get(unsigned offset, unsigned size, void* dst) {
    if (not buffer) {
        return false;
    }
    buffer->getSubData(offset, size, dst);
    return true;
}

bool BufferRawAccessorSubData::change(unsigned offset,
                                      unsigned size,
                                      const Changer& changer) {
    if (not buffer) {
        return false;
    }
    void* data = std::malloc(size);
    get(offset, size, data);
    changer(data, size);
    set(offset, size, data);
    return true;
}

bool BufferRawAccessorSubData::copy(unsigned src_offset,
                                    unsigned dst_offset,
                                    unsigned size) {
    if (not buffer) {
        return false;
    }
    buffer->copyTo(*buffer, src_offset, dst_offset, size);
    return true;
}

} // namespace gnev::gl