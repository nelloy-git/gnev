#pragma once

#include <functional>
#include <memory>

#include "gl/Buffer.hpp"
#include "util/Export.hpp"

namespace gnev::gl {

class EXPORT IBufferRawAccessor {
public:
    using Changer = std::function<void(void*, unsigned size)>;

    virtual ~IBufferRawAccessor() = default;

    virtual Buffer* getBuffer() = 0;
    virtual const Buffer* getBuffer() const = 0;

    virtual std::unique_ptr<Buffer> releaseBuffer() = 0;
    virtual void resetBuffer(std::unique_ptr<Buffer>&& buffer) = 0;

    void swapBuffer(IBufferRawAccessor& other) {
        auto other_buffer = other.releaseBuffer();
        other.resetBuffer(this->releaseBuffer());
        this->resetBuffer(std::move(other_buffer));
    }

    inline std::optional<unsigned> handle() const {
        return getBuffer() ? std::optional{getBuffer()->handle()} : std::nullopt;
    }

    virtual bool set(unsigned offset, unsigned size, const void* data) = 0;
    virtual bool get(unsigned offset, unsigned size, void* data) = 0;
    virtual bool change(unsigned offset, unsigned size, const Changer& changer) = 0;
    virtual bool copy(unsigned src_offset, unsigned dst_offset, unsigned size) = 0;
};

} // namespace gnev::gl