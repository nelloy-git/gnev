#pragma once

#include "gl/Buffer.hpp"
#include "util/Ref.hpp"

namespace gnev::gl {

class BufferAccessor {
public:
    using Changer = std::function<void(gl::Buffer&, void*, GLintptr size)>;

    BufferAccessor(Ref<gl::Buffer>);
    virtual ~BufferAccessor() = default;

    Ref<gl::Buffer> buffer;

    virtual void set(GLintptr offset, GLintptr size, const void* data) = 0;
    virtual void get(GLintptr offset, GLintptr size, void* data) = 0;
    virtual void change(GLintptr offset, GLintptr size, const Changer& changer) = 0;

    GLuint getBufferSize() const;
};

class BufferAccessorSubData : public BufferAccessor {
public:
    BufferAccessorSubData(Ref<gl::Buffer>);
    virtual ~BufferAccessorSubData() = default;

    void set(GLintptr offset, GLintptr size, const void* src) override;
    void get(GLintptr offset, GLintptr size, void* dst) override;
    void change(GLintptr offset, GLintptr size, const Changer& changer) override;
};

class BufferAccessorCoherent : public BufferAccessor {
public:
    BufferAccessorCoherent(Ref<gl::Buffer>);
    virtual ~BufferAccessorCoherent();

    void set(GLintptr offset, GLintptr size, const void* src) override;
    void get(GLintptr offset, GLintptr size, void* dst) override;
    void change(GLintptr offset, GLintptr size, const Changer& changer) override;

private:
    GLbyte* map;
};

} // namespace gnev::gl