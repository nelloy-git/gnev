#pragma once

#include "gl/BufferIndexMapViewElement.hpp"
#include "material/base/MaterialDataStorage.hpp"
#include "material/base/MaterialGL.hpp"

namespace gnev::base {

template <IsMaterialGL T>
class EXPORT MaterialData : public gl::BufferIndexMapViewElement<T> {
public:
    MaterialData(WeakRef<MaterialDataStorage<T>> weak_storage, const T& initial = T{});
    virtual ~MaterialData() = default;

    WeakRef<MaterialDataStorage<T>> getWeakStorage() const;
};

template <IsMaterialGL T>
MaterialData<T>::MaterialData(WeakRef<MaterialDataStorage<T>> weak_storage,
                              const T& initial)
    : gl::BufferIndexMapViewElement<T>(weak_storage, initial) {}

template <IsMaterialGL T>
WeakRef<MaterialDataStorage<T>> MaterialData<T>::getWeakStorage() const {
    return gl::BufferIndexMapViewElement<T>::getWeakStorage().lock().dynamicCast <
           MaterialDataStorage<T>();
}

} // namespace gnev::base