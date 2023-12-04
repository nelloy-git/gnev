#pragma once

#include <concepts>
#include <type_traits>

#include "gl/Buffer.hpp"
#include "util/Ref.hpp"

namespace gnev {

template <typename T, typename Setter, typename Getter>
class EXPORT BufferWrapper {
public:
    BufferWrapper(Ref<gl::Buffer> buffer, const Setter& setter, const Getter& getter);
    virtual ~BufferWrapper();

    Ref<gl::Buffer> getBuffer() const;
    void setSetter(const Setter& setter);
    void setGetter(const Getter& getter);

    template <typename... Args>
    auto setData(Args&&... args);

    template <typename... Args>
    auto getData(Args&&... args) const;

    template <typename V>
    static unsigned int calcCapacity(gl::Buffer& buffer);

private:
    Ref<gl::Buffer> buffer;
    Setter buffer_setter;
    Getter buffer_getter;
};

template <typename T, typename Setter, typename Getter>
BufferWrapper<T, Setter, Getter>::BufferWrapper(Ref<gl::Buffer> buffer,
                                                const Setter& setter,
                                                const Getter& getter)
    : buffer(buffer)
    , buffer_setter(setter)
    , buffer_getter(getter){};

template <typename T, typename Setter, typename Getter>
BufferWrapper<T, Setter, Getter>::~BufferWrapper(){};

template <typename T, typename Setter, typename Getter>
Ref<gl::Buffer> BufferWrapper<T, Setter, Getter>::getBuffer() const {
    return buffer;
};

template <typename T, typename Setter, typename Getter>
void BufferWrapper<T, Setter, Getter>::setSetter(const Setter& setter) {
    buffer_setter = setter;
};

template <typename T, typename Setter, typename Getter>
void BufferWrapper<T, Setter, Getter>::setGetter(const Getter& getter) {
    buffer_getter = getter;
};

template <typename T, typename Setter, typename Getter>
template <typename... Args>
auto BufferWrapper<T, Setter, Getter>::setData(Args&&... args) {
    return buffer_setter(*dynamic_cast<T*>(this), std::forward<Args>(args)...);
}

template <typename T, typename Setter, typename Getter>
template <typename... Args>
auto BufferWrapper<T, Setter, Getter>::getData(Args&&... args) const {
    return buffer_getter(*dynamic_cast<const T*>(this), std::forward<Args>(args)...);
}

template <typename T, typename Setter, typename Getter>
template <typename V>
unsigned int BufferWrapper<T, Setter, Getter>::calcCapacity(gl::Buffer& buffer) {
    GLuint buf_size;
    buffer.getParameteriv(GL_BUFFER_SIZE, (GLint*)&buf_size);
    return buf_size / sizeof(V);
}

} // namespace gnev