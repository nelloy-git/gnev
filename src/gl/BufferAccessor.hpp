#pragma once

#include "gl/Buffer.hpp"
#include "util/Ref.hpp"

namespace gnev::gl {

class BufferAccessor {
public:
    using Changer = std::function<bool(gl::Buffer&, void*, GLintptr size)>;

    BufferAccessor(Ref<gl::Buffer>);
    virtual ~BufferAccessor() = default;

    Ref<gl::Buffer> buffer;

    virtual bool set(GLintptr offset, GLintptr size, const void* data) = 0;
    virtual bool get(GLintptr offset, GLintptr size, void* data) = 0;
    virtual bool change(GLintptr offset, GLintptr size, const Changer& changer) = 0;

    GLuint getBufferSize() const;
};

class BufferAccessorSubData : public BufferAccessor {
public:
    BufferAccessorSubData(Ref<gl::Buffer>);
    virtual ~BufferAccessorSubData() = default;

    bool set(GLintptr offset, GLintptr size, const void* src) override;
    bool get(GLintptr offset, GLintptr size, void* dst) override;
    bool change(GLintptr offset, GLintptr size, const Changer& changer) override;
};

class BufferAccessorCoherent : public BufferAccessor {
public:
    BufferAccessorCoherent(Ref<gl::Buffer>);
    virtual ~BufferAccessorCoherent();

    bool set(GLintptr offset, GLintptr size, const void* src) override;
    bool get(GLintptr offset, GLintptr size, void* dst) override;
    bool change(GLintptr offset, GLintptr size, const Changer& changer) override;

private:
    GLbyte* map;
};

} // namespace gnev::gl