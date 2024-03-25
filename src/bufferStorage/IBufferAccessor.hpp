#pragma once

#include <functional>

#include "gl/Buffer.hpp"
#include "util/Export.hpp"

namespace gnev::gl {

class EXPORT IBufferAccessor {
public:
    using Changer = std::function<void(void*, unsigned size)>;

    virtual ~IBufferAccessor() = default;

    virtual void bindBuffer(Buffer* buffer);
    virtual Buffer* getBoundBuffer() const;

    virtual bool set(unsigned offset, unsigned size, const void* data) = 0;
    virtual bool get(unsigned offset, unsigned size, void* data) = 0;
    virtual bool change(unsigned offset, unsigned size, const Changer& changer) = 0;
    virtual bool copy(unsigned src_offset, unsigned dst_offset, unsigned size) = 0;
};

} // namespace gnev::gl