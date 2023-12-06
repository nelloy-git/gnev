#pragma once

#include "gl/BufferIndexMapView.hpp"
#include "transform/base/TransformGL.hpp"

namespace gnev::base {

template <IsTransformGL T>
class EXPORT TransformStorage : public gl::BufferIndexMapView<T> {
public:
    TransformStorage(Ref<gl::BufferAccessor> accessor);
    TransformStorage(GLuint capacity);
    virtual ~TransformStorage() = default;

private:
    static Ref<gl::BufferAccessor> initAccessor(GLuint capacity);
};

template <IsTransformGL T>
TransformStorage<T>::TransformStorage(Ref<gl::BufferAccessor> accessor)
    : gl::BufferIndexMapView<T>(accessor) {}

template <IsTransformGL T>
TransformStorage<T>::TransformStorage(GLuint capacity)
    : gl::BufferIndexMapView<T>(initAccessor(capacity)) {}

template <IsTransformGL T>
Ref<gl::BufferAccessor> TransformStorage<T>::initAccessor(GLuint capacity) {
    auto buffer = MakeSharable<gl::Buffer>();
    buffer->initStorage(capacity * sizeof(T),
                        nullptr,
                        GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT |
                            GL_MAP_COHERENT_BIT);
    return MakeSharable<gl::BufferAccessorCoherent>(buffer);
}

} // namespace gnev::base
