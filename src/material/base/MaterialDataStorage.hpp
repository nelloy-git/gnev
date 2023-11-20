#pragma once

#include "gl/buffer/BufStorageIndexMap.hpp"
#include "gl/buffer/BufStorageIterator.hpp"
#include "material/base/Define.hpp"
#include "material/base/MaterialGL.hpp"

namespace gnev::base {

template <IsMaterialGL T>
class EXPORT MaterialDataStorage {
public:
    MaterialDataStorage(GLbitfield storage_flags,
                        GLuint capacity,
                        const gl::BufStorageIndexMap<T>::CleanUp& cleanup = std::nullopt);
    ~MaterialDataStorage();

    std::optional<MaterialDataIndex> initIndex();
    void freeIndex(MaterialDataIndex index);

    gl::BufStorageIterator<T> at(MaterialDataIndex index);
    const gl::BufStorageIterator<T> at(MaterialDataIndex index) const;

private:
    gl::BufStorageIndexMap<T> storage;
};

template <IsMaterialGL T>
MaterialDataStorage<T>::MaterialDataStorage(GLbitfield storage_flags,
                                            GLuint capacity,
                                            const gl::BufStorageIndexMap<T>::CleanUp&
                                                cleanup)
    : storage(storage_flags, capacity, cleanup) {}

template <IsMaterialGL T>
MaterialDataStorage<T>::~MaterialDataStorage() {}

template <IsMaterialGL T>
gl::BufStorageIterator<T> MaterialDataStorage<T>::at(MaterialDataIndex index) {
    return storage.at(index);
}

template <IsMaterialGL T>
const gl::BufStorageIterator<T>
MaterialDataStorage<T>::at(MaterialDataIndex index) const {
    return storage.at(index);
}

template <IsMaterialGL T>
std::optional<MaterialDataIndex> MaterialDataStorage<T>::initIndex() {
    return storage.initUnusedIndex();
}

template <IsMaterialGL T>
void MaterialDataStorage<T>::freeIndex(MaterialDataIndex index) {
    storage.freeIndex(index);
}
} // namespace gnev::base