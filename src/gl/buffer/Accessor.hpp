#pragma once

#include "gl/Buffer.hpp"
#include "util/Ref.hpp"

namespace gnev::gl::buffer {

class Accessor {
public:
    using Changer = std::function<void(void*, GLintptr size)>;

    Accessor(Ref<gl::Buffer>);
    virtual ~Accessor() = default;

    Ref<gl::Buffer> buffer;

    virtual void set(GLintptr offset, GLintptr size, const void* data) = 0;
    virtual void get(GLintptr offset, GLintptr size, void* data) = 0;
    virtual void change(GLintptr offset, GLintptr size, const Changer& changer) = 0;

    GLuint getBufferSize() const;
};

class AccessorSubData : public Accessor {
public:
    AccessorSubData(Ref<gl::Buffer>);
    virtual ~AccessorSubData() = default;

    void set(GLintptr offset, GLintptr size, const void* src) override;
    void get(GLintptr offset, GLintptr size, void* dst) override;
    void change(GLintptr offset, GLintptr size, const Changer& changer) override;
};

class AccessorCoherent : public Accessor {
public:
    AccessorCoherent(Ref<gl::Buffer>);
    virtual ~AccessorCoherent();

    void set(GLintptr offset, GLintptr size, const void* src) override;
    void get(GLintptr offset, GLintptr size, void* dst) override;
    void change(GLintptr offset, GLintptr size, const Changer& changer) override;

private:
    GLbyte* map;
};

} // namespace gnev::gl