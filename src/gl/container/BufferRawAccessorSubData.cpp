#include "gl/container/BufferRawAccessorSubData.hpp"

#include "util/Logger.hpp"

namespace gnev::gl {

BufferRawAccessorSubData::BufferRawAccessorSubData(std::unique_ptr<Buffer>&& buffer) {
    resetBuffer(std::move(buffer));
}

Buffer& BufferRawAccessorSubData::getBuffer() { return *buffer; }

const Buffer& BufferRawAccessorSubData::getBuffer() const { return *buffer; }

std::unique_ptr<Buffer> BufferRawAccessorSubData::releaseBuffer() {
    return std::move(buffer);
}

void BufferRawAccessorSubData::resetBuffer(std::unique_ptr<Buffer>&& buffer_) {
    using enum BufferStorageFlags;
    buffer = std::move(buffer_);
    if (buffer and buffer->isStorage()) {
        auto storage_flags = buffer->getStorageFlags();
        if ((storage_flags & DYNAMIC_STORAGE_BIT) != EMPTY) {
            Logger::WARNING<
                "Buffer<{}> can not be used correctly (StorageFlags = {})">(buffer
                                                                                ->handle(),
                                                                            storage_flags);
        }
    }
}

bool BufferRawAccessorSubData::set(unsigned offset, unsigned size, const void* src) {
    if (not buffer) {
        Logger::WARNING<"Buffer pointer is empty">();
        return false;
    }
    buffer->setSubData(offset, size, src);
    return true;
}

bool BufferRawAccessorSubData::get(unsigned offset, unsigned size, void* dst) {
    if (not buffer) {
        Logger::WARNING<"Buffer pointer is empty">();
        return false;
    }
    buffer->getSubData(offset, size, dst);
    return true;
}

bool BufferRawAccessorSubData::change(unsigned offset,
                                      unsigned size,
                                      const Changer& changer) {
    if (not buffer) {
        Logger::WARNING<"Buffer pointer is empty">();
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
        Logger::WARNING<"Buffer pointer is empty">();
        return false;
    }
    buffer->copyTo(*buffer, src_offset, dst_offset, size);
    return true;
}

} // namespace gnev::gl