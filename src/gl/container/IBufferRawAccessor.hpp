#pragma once

#include <functional>

#include "gl/Buffer.hpp"
#include "util/Export.hpp"

namespace gnev::gl {

class EXPORT IBufferRawAccessor {
public:
    using Changer = std::function<void(void*, unsigned size)>;

    virtual ~IBufferRawAccessor() = default;

    virtual void bindBuffer(Buffer* buffer) = 0;
    virtual Buffer* getBoundBuffer() const = 0;

    virtual bool set(unsigned offset, unsigned size, const void* data) = 0;
    virtual bool get(unsigned offset, unsigned size, void* data) = 0;
    virtual bool change(unsigned offset, unsigned size, const Changer& changer) = 0;
    virtual bool copy(unsigned src_offset, unsigned dst_offset, unsigned size) = 0;
};

} // namespace gnev::gl