#pragma once

#include "gl/container/IBufferRawAccessor.hpp"

namespace gnev::gl {

class EXPORT BufferRawAccessorMappedCoherent : public IBufferRawAccessor {
public:
    ~BufferRawAccessorMappedCoherent();

    void bindBuffer(Buffer* buffer) override;
    Buffer* getBoundBuffer() const override;

    bool set(unsigned offset, unsigned size, const void* data) override;
    bool get(unsigned offset, unsigned size, void* data) override;
    bool change(unsigned offset, unsigned size, const Changer& changer) override;
    bool copy(unsigned src_offset, unsigned dst_offset, unsigned size) override;

private:
    Buffer* buffer = nullptr;
    GLbyte* map = nullptr;
};

} // namespace gnev::gl