#pragma once

#include <concepts>
#include <type_traits>

#include "gl/Buffer.hpp"
#include "gl/BufferAccessor.hpp"
#include "util/Ref.hpp"

namespace gnev::gl {

template <typename T>
class EXPORT BufferArrayView {
public:
    template <typename V>
    using Changer = std::function<bool(gl::Buffer&, V&)>;

    BufferArrayView(Ref<BufferAccessor> accessor);
    virtual ~BufferArrayView() = default;

    Ref<Buffer> getBuffer() const;
    void setAccessor(Ref<BufferAccessor> accessor);

    bool set(GLuint index, const T& src)
        requires(not std::is_pointer_v<T>);

    template <typename V>
    bool set(GLuint index, const V& src, GLintptr ptr_offset)
        requires(not std::is_pointer_v<V>);

    bool get(GLuint index, T& dst) const
        requires(not std::is_pointer_v<T>);

    template <typename V>
    bool get(GLuint index, V& dst, GLintptr ptr_offset) const
        requires(not std::is_pointer_v<V>);

    bool change(GLuint index, const Changer<T>& changer)
        requires(not std::is_pointer_v<T>);

    template <typename V>
    bool change(GLuint index, const Changer<V>& changer, GLintptr ptr_offset)
        requires(not std::is_pointer_v<V>);

private:
    Ref<BufferAccessor> accessor;
};

template <typename T>
BufferArrayView<T>::BufferArrayView(Ref<BufferAccessor> accessor)
    : accessor(accessor){};

template <typename T>
Ref<Buffer> BufferArrayView<T>::getBuffer() const {
    return accessor->buffer;
};

template <typename T>
void BufferArrayView<T>::setAccessor(Ref<BufferAccessor> other) {
    if (accessor->buffer != other->buffer) {
        throw std::logic_error("");
    }
    accessor = other;
};

template <typename T>
bool BufferArrayView<T>::set(GLuint index, const T& src)
    requires(not std::is_pointer_v<T>)
{
    return accessor->set(index * sizeof(T), sizeof(T), &src);
}

template <typename T>
template <typename V>
bool BufferArrayView<T>::set(GLuint index, const V& src, GLintptr ptr_offset)
    requires(not std::is_pointer_v<V>)
{
    return accessor->set(index * sizeof(T) + ptr_offset, sizeof(V), &src);
}

template <typename T>
bool BufferArrayView<T>::get(GLuint index, T& dst) const
    requires(not std::is_pointer_v<T>)
{
    return accessor->get(index * sizeof(T), sizeof(T), &dst);
}

template <typename T>
template <typename V>
bool BufferArrayView<T>::get(GLuint index, V& dst, GLintptr ptr_offset) const
    requires(not std::is_pointer_v<V>)
{
    return accessor->get(index * sizeof(T) + ptr_offset, sizeof(V), &dst);
}

template <typename T>
bool BufferArrayView<T>::change(GLuint index, const Changer<T>& changer)
    requires(not std::is_pointer_v<T>)
{
    auto wrapper = [changer](gl::Buffer& buffer, void* data, GLintptr) -> bool {
        changer(buffer, *static_cast<T*>(data));
    };
    return accessor->change(index * sizeof(T), sizeof(T), wrapper);
}

template <typename T>
template <typename V>
bool BufferArrayView<T>::change(GLuint index,
                                const Changer<V>& changer,
                                GLintptr ptr_offset)
    requires(not std::is_pointer_v<V>)
{
    auto wrapper = [changer](gl::Buffer& buffer, void* data, GLintptr) -> bool {
        changer(buffer, *static_cast<V*>(data));
    };
    return accessor->change(index * sizeof(T) + ptr_offset, sizeof(V), changer);
}

} // namespace gnev::gl