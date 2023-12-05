#pragma once

#include "gl/BufferArrayView.hpp"
#include "transform/TransformGL.hpp"
#include "util/IndexStorage.hpp"

namespace gnev::base {

template <IsTransformGL T>
class EXPORT TransformStorage
    : public IndexStorage
    , public gl::BufferArrayView<T> {
public:
    template <typename V>
    using Changer = gl::BufferArrayView<T>::template Changer<V>;

    TransformStorage(GLuint capacity);
    virtual ~TransformStorage();

    bool set(GLuint index, const T& src);

    template <typename V>
    bool set(GLuint index, const V& src, GLintptr ptr_offset);

    bool get(GLuint index, T& dst) const;

    template <typename V>
    bool get(GLuint index, V& dst, GLintptr ptr_offset) const;

    bool change(GLuint index, const Changer<T>& changer);

    template <typename V>
    bool change(GLuint index, const Changer<V>& changer, GLintptr ptr_offset);

private:
    static Ref<gl::BufferAccessor> initAccessor(GLuint capacity);
};

} // namespace gnev::base
