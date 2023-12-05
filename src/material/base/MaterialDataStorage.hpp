#pragma once

#include "gl/BufferIndexMapView.hpp"
#include "material/base/MaterialGL.hpp"

namespace gnev::base {

template <IsMaterialGL T>
class EXPORT MaterialDataStorage : public gl::BufferIndexMapView<T> {
public:
    MaterialDataStorage(Ref<gl::BufferAccessor> accessor);
    MaterialDataStorage(GLuint capacity);
    virtual ~MaterialDataStorage() = default;

private:
    static Ref<gl::BufferAccessor> initAccessor(GLuint capacity);
};

template <IsMaterialGL T>
MaterialDataStorage<T>::MaterialDataStorage(Ref<gl::BufferAccessor> accessor)
    : gl::BufferIndexMapView<T>(accessor) {}

template <IsMaterialGL T>
MaterialDataStorage<T>::MaterialDataStorage(GLuint capacity)
    : gl::BufferIndexMapView<T>(initAccessor(capacity)) {}

template <IsMaterialGL T>
Ref<gl::BufferAccessor> MaterialDataStorage<T>::initAccessor(GLuint capacity) {
    auto buffer = MakeSharable<gl::Buffer>();
    buffer->initStorage(capacity * sizeof(T),
                        nullptr,
                        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT);
    return MakeSharable<gl::BufferAccessorSubData>(buffer);
}

} // namespace gnev::base