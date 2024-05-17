#pragma once

#include "gl/container/IBufferAccessor.hpp"

namespace gnev::gl {

class EXPORT BufferAccessorSubData : public IBufferAccessor {
public:
    bool set(Buffer& buffer, unsigned offset, unsigned size, const void* data) override;
    bool get(Buffer& buffer, unsigned offset, unsigned size, void* data) override;
    bool change(Buffer& buffer, unsigned offset, unsigned size, const Changer& changer) override;
    bool copy(Buffer& buffer, unsigned src_offset, unsigned dst_offset, unsigned size) override;
};

} // namespace gnev::gl