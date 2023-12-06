#pragma once

#include "gl/BufferIndexMapViewElement.hpp"
#include "transform/base/TransformGL.hpp"
#include "transform/base/TransformStorage.hpp"

namespace gnev::base {

template <IsTransformGL T>
class EXPORT Transform : public gl::BufferIndexMapViewElement<T> {
    friend class TransformMap;

public:
    using Data = T;

    Transform(WeakRef<TransformStorage<T>> weak_storage);
    ~Transform() = default;

    WeakRef<TransformStorage<T>> getWeakStorage() const;

    std::optional<Ref<Transform<T>>> getParentRef() const;
    void setParentRef(const std::optional<Ref<Transform<T>>>& parent);

private:
    std::optional<Ref<Transform<T>>> parent;
};

template <IsTransformGL T>
Transform<T>::Transform(WeakRef<TransformStorage<T>> weak_storage)
    : gl::BufferIndexMapViewElement<T>(weak_storage) {}

template <IsTransformGL T>
WeakRef<TransformStorage<T>> Transform<T>::getWeakStorage() const {
    return gl::BufferIndexMapViewElement<T>::getWeakStorage().lock().dynamicCast <
           TransformStorage<T>();
}

template <IsTransformGL T>
std::optional<Ref<Transform<T>>> Transform<T>::getParentRef() const {
    return parent;
}

template <IsTransformGL T>
void Transform<T>::setParentRef(const std::optional<Ref<Transform<T>>>& parent_ref) {
    GLuint parent_index =
        parent_ref.has_value() ? *parent_ref.value()->getIndex() : T::InvalidIndex;
    gl::BufferIndexMapViewElement<T>::set(parent_index, offsetof(T, parent));
}

} // namespace gnev::base