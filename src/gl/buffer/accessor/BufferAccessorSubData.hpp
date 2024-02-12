#pragma once

#include "gl/buffer/accessor/IBufferAccessor.hpp"

namespace gnev::gl {

class EXPORT BufferAccessorSubData : public IBufferAccessor {
public:
    BufferAccessorSubData(Ref<Buffer>& buffer);

    Ref<gl::Buffer>& getBuffer() const override;

    void set(GLintptr offset, GLintptr size, const void* data) override;
    void get(GLintptr offset, GLintptr size, void* data) override;
    void change(GLintptr offset, GLintptr size, const Changer& changer) override;

private:
    Ref<Buffer>& buffer;
};

} // namespace gnev::gl