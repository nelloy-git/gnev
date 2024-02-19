#include "gl/buffer/BufferAccessorSubData.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "util/InstanceLogger.hpp"

namespace gnev::gl {

BufferAccessorSubData::BufferAccessorSubData(std::unique_ptr<Buffer>&& buffer)
    : buffer{std::move(buffer)} {

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

const Buffer& BufferAccessorSubData::getBuffer() const { return *buffer; }

void BufferAccessorSubData::set(GLintptr offset, GLintptr size, const void* src) {
    buffer->setSubData(offset, size, src);
}

void BufferAccessorSubData::get(GLintptr offset, GLintptr size, void* dst) {
    buffer->getSubData(offset, size, dst);
}

void BufferAccessorSubData::change(GLintptr offset,
                                   GLintptr size,
                                   const Changer& changer) {
    void* data = std::malloc(size);
    get(offset, size, data);
    changer(data, size);
    set(offset, size, data);
}

} // namespace gnev::gl