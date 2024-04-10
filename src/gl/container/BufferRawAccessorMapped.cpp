#include "gl/container/BufferRawAccessorMapped.hpp"

#include "gl/enum/BufferMapRangeAccess.hpp"
#include "util/Logger.hpp"

namespace gnev::gl {

BufferRawAccessorMapped::BufferRawAccessorMapped(BufferMapRangeAccess access,
                                                 std::unique_ptr<Buffer>&& buffer)
    : access{access} {
    resetBuffer(std::move(buffer));
}

Buffer& BufferRawAccessorMapped::getBuffer() { return *buffer; }

const Buffer& BufferRawAccessorMapped::getBuffer() const { return *buffer; }

std::unique_ptr<Buffer> BufferRawAccessorMapped::releaseBuffer() {
    if (buffer) {
        buffer->unmap();
        map = nullptr;
    }
    return std::move(buffer);
}

void BufferRawAccessorMapped::resetBuffer(std::unique_ptr<Buffer>&& buffer_) {
    if (buffer) {
        buffer->unmap();
        map = nullptr;
    }

    buffer = std::move(buffer_);
    if (not buffer) {
        return;
    }
    map = static_cast<GLbyte*>(buffer->mapRange(0, buffer->getSize(), access));
    if (not map) {
        Ctx::Get()
            .getLogger()
            .log<LogLevel::WARNING, "Failed map Buffer<{}>">(buffer->handle());
    }
}

BufferRawAccessorMapped::~BufferRawAccessorMapped() {
    if (buffer and map) {
        buffer->unmap();
    }
}

bool BufferRawAccessorMapped::set(unsigned offset, unsigned size, const void* src) {
    if (not map) {
        return false;
    }
    std::memcpy(map + offset, src, size);
    return true;
}

bool BufferRawAccessorMapped::get(unsigned offset, unsigned size, void* dst) {
    if (not map) {
        return false;
    }
    std::memcpy(dst, map + offset, size);
    return true;
}

bool BufferRawAccessorMapped::change(unsigned offset,
                                     unsigned size,
                                     const Changer& changer) {
    if (not map) {
        return false;
    }
    changer(map + offset, size);
    return true;
}

bool BufferRawAccessorMapped::copy(unsigned src_offset,
                                   unsigned dst_offset,
                                   unsigned size) {
    if (not map) {
        return false;
    }
    std::memcpy(map + dst_offset, map + src_offset, size);
    return true;
}

} // namespace gnev::gl