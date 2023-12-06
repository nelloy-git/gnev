#pragma once

#include "gl/BufferAccessor.hpp"
#include "gl/BufferArrayView.hpp"
#include "util/IndexStorage.hpp"
#include "util/Ref.hpp"

namespace gnev::gl {

template <typename T>
class EXPORT BufferIndexMapView
    : public IndexStorage
    , public gl::BufferArrayView<T> {
public:
    template <typename V>
    using Changer = gl::BufferArrayView<T>::template Changer<V>;

    BufferIndexMapView(Ref<gl::BufferAccessor> accessor);
    virtual ~BufferIndexMapView() = default;

    void set(GLuint index, const T& src);

    template <typename V>
    void set(GLuint index, const V& src, GLintptr ptr_offset);

    void get(GLuint index, T& dst) const;

    template <typename V>
    void get(GLuint index, V& dst, GLintptr ptr_offset) const;

    void change(GLuint index, const Changer<T>& changer);

    template <typename V>
    void change(GLuint index, const Changer<V>& changer, GLintptr ptr_offset);
};

template <typename T>
BufferIndexMapView<T>::BufferIndexMapView(Ref<gl::BufferAccessor> accessor)
    : IndexStorage(accessor->getBufferSize() / sizeof(T))
    , gl::BufferArrayView<T>(accessor) {}

template <typename T>
void BufferIndexMapView<T>::set(GLuint index, const T& src) {
    if (not isUsed(index)) {
        throw std::out_of_range("");
    }
    gl::BufferArrayView<T>::set(index, src);
}

template <typename T>
template <typename V>
void BufferIndexMapView<T>::set(GLuint index, const V& src, GLintptr ptr_offset) {
    if (not isUsed(index) or sizeof(V) + ptr_offset >= sizeof(T)) {
        throw std::out_of_range("");
    }
    gl::BufferArrayView<T>::set(index, src, ptr_offset);
}

template <typename T>
void BufferIndexMapView<T>::get(GLuint index, T& dst) const {
    gl::BufferArrayView<T>::get(index, dst);
}

template <typename T>
template <typename V>
void BufferIndexMapView<T>::get(GLuint index, V& dst, GLintptr ptr_offset) const {
    if (not isUsed(index) or sizeof(V) + ptr_offset >= sizeof(T)) {
        throw std::out_of_range("");
    }
    gl::BufferArrayView<T>::get(index, dst, ptr_offset);
}

template <typename T>
void BufferIndexMapView<T>::change(GLuint index, const Changer<T>& changer) {
    if (not isUsed(index)) {
        throw std::out_of_range("");
    }
    gl::BufferArrayView<T>::change(index, changer);
}

template <typename T>
template <typename V>
void BufferIndexMapView<T>::change(GLuint index,
                                   const Changer<V>& changer,
                                   GLintptr ptr_offset) {
    if (not isUsed(index) or sizeof(V) + ptr_offset >= sizeof(T)) {
        throw std::out_of_range("");
    }
    gl::BufferArrayView<T>::change(index, changer, ptr_offset);
}

} // namespace gnev::gl