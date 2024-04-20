#pragma once

#include <memory>

#include "gl/container/IBufferRawAccessor.hpp"

namespace gnev::gl {

class EXPORT BufferRawAccessorSubData : public IBufferRawAccessor {
public:
    BufferRawAccessorSubData(std::unique_ptr<Buffer>&& buffer = nullptr);

    Buffer* getBuffer() override;
    const Buffer* getBuffer() const override;

    std::unique_ptr<Buffer> releaseBuffer() override;
    void resetBuffer(std::unique_ptr<Buffer>&& buffer) override;

    bool set(unsigned offset, unsigned size, const void* data) override;
    bool get(unsigned offset, unsigned size, void* data) override;
    bool change(unsigned offset, unsigned size, const Changer& changer) override;
    bool copy(unsigned src_offset, unsigned dst_offset, unsigned size) override;

private:
    std::unique_ptr<Buffer> buffer;
};

} // namespace gnev::gl