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

    bool set(GLuint index, const T& src);

    template <typename V>
    bool set(GLuint index, const V& src, GLintptr ptr_offset);

    bool get(GLuint index, T& dst) const;

    template <typename V>
    bool get(GLuint index, V& dst, GLintptr ptr_offset) const;

    bool change(GLuint index, const Changer<T>& changer);

    template <typename V>
    bool change(GLuint index, const Changer<V>& changer, GLintptr ptr_offset);
};

template <typename T>
BufferIndexMapView<T>::BufferIndexMapView(Ref<gl::BufferAccessor> accessor)
    : IndexStorage(accessor->getBufferSize() / sizeof(T))
    , gl::BufferArrayView<T>(accessor) {}

template <typename T>
bool BufferIndexMapView<T>::set(GLuint index, const T& src) {
    if (not isUsed(index)) {
        throw std::out_of_range("");
    }
    return gl::BufferArrayView<T>::set(index, src);
}

template <typename T>
template <typename V>
bool BufferIndexMapView<T>::set(GLuint index, const V& src, GLintptr ptr_offset) {
    if (not isUsed(index) or sizeof(V) + ptr_offset >= sizeof(T)) {
        throw std::out_of_range("");
    }
    return gl::BufferArrayView<T>::set(index, src, ptr_offset);
}

template <typename T>
bool BufferIndexMapView<T>::get(GLuint index, T& dst) const {
    return gl::BufferArrayView<T>::get(index, dst);
}

template <typename T>
template <typename V>
bool BufferIndexMapView<T>::get(GLuint index, V& dst, GLintptr ptr_offset) const {
    if (not isUsed(index) or sizeof(V) + ptr_offset >= sizeof(T)) {
        throw std::out_of_range("");
    }
    return gl::BufferArrayView<T>::get(index, dst, ptr_offset);
}

template <typename T>
bool BufferIndexMapView<T>::change(GLuint index, const Changer<T>& changer) {
    if (not isUsed(index)) {
        throw std::out_of_range("");
    }

    auto wrapper = [changer](gl::Buffer& buffer, void* data, GLintptr) -> bool {
        changer(buffer, *static_cast<T*>(data));
    };
    return gl::BufferArrayView<T>::change(index, wrapper);
}

template <typename T>
template <typename V>
bool BufferIndexMapView<T>::change(GLuint index,
                                    const Changer<V>& changer,
                                    GLintptr ptr_offset) {
    if (not isUsed(index) or sizeof(V) + ptr_offset >= sizeof(T)) {
        throw std::out_of_range("");
    }

    auto wrapper = [changer](gl::Buffer& buffer, void* data, GLintptr) -> bool {
        changer(buffer, *static_cast<V*>(data));
    };
    return gl::BufferArrayView<T>::change(index, wrapper, ptr_offset);
}

} // namespace gnev::base