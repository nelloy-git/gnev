#pragma once

#include "gl/buffer/IndexMapViewElem.hpp"
#include "transform/base/TransformGL.hpp"

namespace gnev::base {

template <IsTransformGL T>
class EXPORT Transform {
public:
    using DataView = gl::buffer::IndexMapView<T>;
    using DataElem = gl::buffer::IndexMapViewElem<T>;

    static constexpr GLuint InvalidIndex = T::InvalidIndex;

    Transform(WeakRef<DataView> weak_view);
    virtual ~Transform() = default;

protected:
    Ref<DataElem> data;
    Ptr<Transform<T>> parent;

    void setParent(const Ptr<Transform<T>>& parent);
    Ptr<Transform<T>> getParent() const;
};

template <IsTransformGL T>
Transform<T>::Transform(WeakRef<DataView> weak_view)
    : data(MakeSharable<DataElem>(weak_view)) {}

template <IsTransformGL T>
void Transform<T>::setParent(const Ptr<Transform<T>>& parent) {
    this->parent = parent;
    GLuint index = parent ? *parent->data->getIndex() : InvalidIndex;
    data->set(index, offsetof(T, parent));
}

template <IsTransformGL T>
Ptr<Transform<T>> Transform<T>::getParent() const {
    return parent;
}

} // namespace gnev::base