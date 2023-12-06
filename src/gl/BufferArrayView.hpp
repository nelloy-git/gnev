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
    using Changer = std::function<void(V&)>;

    BufferArrayView(Ref<BufferAccessor> accessor);
    virtual ~BufferArrayView() = default;

    Ref<Buffer> getBuffer() const;
    void setAccessor(Ref<BufferAccessor> accessor);

    void set(GLuint index, const T& src)
        requires(not std::is_pointer_v<T>);

    template <typename V>
    void set(GLuint index, const V& src, GLintptr ptr_offset)
        requires(not std::is_pointer_v<V>);

    void get(GLuint index, T& dst) const
        requires(not std::is_pointer_v<T>);

    template <typename V>
    void get(GLuint index, V& dst, GLintptr ptr_offset) const
        requires(not std::is_pointer_v<V>);

    void change(GLuint index, const Changer<T>& changer)
        requires(not std::is_pointer_v<T>);

    template <typename V>
    void change(GLuint index, const Changer<V>& changer, GLintptr ptr_offset)
        requires(not std::is_pointer_v<V>);

private:
    Ref<BufferAccessor> accessor;
};

template <typename T>
BufferArrayView<T>::BufferArrayView(Ref<BufferAccessor> accessor)
    : accessor(accessor){};

template <typename T>
Ref<Buffer> BufferArrayView<T>::getBuffer() const {
    accessor->buffer;
};

template <typename T>
void BufferArrayView<T>::setAccessor(Ref<BufferAccessor> other) {
    if (accessor->buffer != other->buffer) {
        throw std::logic_error("");
    }
    accessor = other;
};

template <typename T>
void BufferArrayView<T>::set(GLuint index, const T& src)
    requires(not std::is_pointer_v<T>)
{
    accessor->set(index * sizeof(T), sizeof(T), &src);
}

template <typename T>
template <typename V>
void BufferArrayView<T>::set(GLuint index, const V& src, GLintptr ptr_offset)
    requires(not std::is_pointer_v<V>)
{
    accessor->set(index * sizeof(T) + ptr_offset, sizeof(V), &src);
}

template <typename T>
void BufferArrayView<T>::get(GLuint index, T& dst) const
    requires(not std::is_pointer_v<T>)
{
    accessor->get(index * sizeof(T), sizeof(T), &dst);
}

template <typename T>
template <typename V>
void BufferArrayView<T>::get(GLuint index, V& dst, GLintptr ptr_offset) const
    requires(not std::is_pointer_v<V>)
{
    accessor->get(index * sizeof(T) + ptr_offset, sizeof(V), &dst);
}

template <typename T>
void BufferArrayView<T>::change(GLuint index, const Changer<T>& changer)
    requires(not std::is_pointer_v<T>)
{
    auto wrapper = [changer](gl::Buffer&, void* data, GLintptr) -> void {
        changer(*static_cast<T*>(data));
    };
    accessor->change(index * sizeof(T), sizeof(T), wrapper);
}

template <typename T>
template <typename V>
void BufferArrayView<T>::change(GLuint index,
                                const Changer<V>& changer,
                                GLintptr ptr_offset)
    requires(not std::is_pointer_v<V>)
{
    auto wrapper = [changer](gl::Buffer&, void* data, GLintptr) -> void {
        changer(*static_cast<V*>(data));
    };
    accessor->change(index * sizeof(T) + ptr_offset, sizeof(V), wrapper);
}

} // namespace gnev::gl