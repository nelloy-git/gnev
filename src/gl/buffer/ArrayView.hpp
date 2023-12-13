#pragma once

#include <concepts>
#include <type_traits>

#include "gl/buffer/Accessor.hpp"

namespace gnev::gl::buffer {

template <typename T>
concept UsableByArrayView = std::is_trivially_constructible_v<T>;

template <UsableByArrayView T>
class EXPORT ArrayView {
public:
    template <UsableByArrayView V>
    using Changer = std::function<void(V&)>;

    ArrayView(Ref<Accessor> accessor);
    virtual ~ArrayView() = default;

    Ref<Accessor> accessor;

    inline void set(GLuint index, const T& src)
        requires(not std::is_pointer_v<T>);

    template <UsableByArrayView V>
    inline void set(GLuint index, const V& src, GLintptr ptr_offset)
        requires(not std::is_pointer_v<V>);

    inline void get(GLuint index, T& dst) const
        requires(not std::is_pointer_v<T>);

    template <UsableByArrayView V>
    inline void get(GLuint index, V& dst, GLintptr ptr_offset) const
        requires(not std::is_pointer_v<V>);

    void change(GLuint index, const Changer<T>& changer)
        requires(not std::is_pointer_v<T>);

    template <UsableByArrayView V>
    void change(GLuint index, const Changer<V>& changer, GLintptr ptr_offset)
        requires(not std::is_pointer_v<V>);
};

template <UsableByArrayView T>
ArrayView<T>::ArrayView(Ref<Accessor> accessor)
    : accessor(accessor){};

template <UsableByArrayView T>
inline void ArrayView<T>::set(GLuint index, const T& src)
    requires(not std::is_pointer_v<T>)
{
    accessor->set(index * sizeof(T), sizeof(T), &src);
}

template <UsableByArrayView T>
template <UsableByArrayView V>
inline void ArrayView<T>::set(GLuint index, const V& src, GLintptr ptr_offset)
    requires(not std::is_pointer_v<V>)
{
    if (sizeof(V) + ptr_offset >= sizeof(T)) {
        throw std::out_of_range("");
    }
    accessor->set(index * sizeof(T) + ptr_offset, sizeof(V), &src);
}

template <UsableByArrayView T>
inline void ArrayView<T>::get(GLuint index, T& dst) const
    requires(not std::is_pointer_v<T>)
{
    accessor->get(index * sizeof(T), sizeof(T), &dst);
}

template <UsableByArrayView T>
template <UsableByArrayView V>
inline void ArrayView<T>::get(GLuint index, V& dst, GLintptr ptr_offset) const
    requires(not std::is_pointer_v<V>)
{
    if (sizeof(V) + ptr_offset >= sizeof(T)) {
        throw std::out_of_range("");
    }
    accessor->get(index * sizeof(T) + ptr_offset, sizeof(V), &dst);
}

template <UsableByArrayView T>
void ArrayView<T>::change(GLuint index, const Changer<T>& changer)
    requires(not std::is_pointer_v<T>)
{
    auto wrapper = [changer](gl::Buffer&, void* data, GLintptr) -> void {
        changer(*static_cast<T*>(data));
    };
    accessor->change(index * sizeof(T), sizeof(T), wrapper);
}

template <UsableByArrayView T>
template <UsableByArrayView V>
void ArrayView<T>::change(GLuint index, const Changer<V>& changer, GLintptr ptr_offset)
    requires(not std::is_pointer_v<V>)
{
    if (sizeof(V) + ptr_offset >= sizeof(T)) {
        throw std::out_of_range("");
    }
    auto wrapper = [changer](gl::Buffer&, void* data, GLintptr) -> void {
        changer(*static_cast<V*>(data));
    };
    accessor->change(index * sizeof(T) + ptr_offset, sizeof(V), wrapper);
}

} // namespace gnev::gl::buffer