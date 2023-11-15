#pragma once

#include <memory>
#include <unordered_map>

#include "material/base/Defines.hpp"
#include "material/base/ImageLoader.hpp"
#include "material/base/Material.hpp"

namespace gnev::base {

template <IsTriviallyCopyable T>
class EXPORT MaterialStorage {
public:
    using DataStorage = MaterialDataStorage<T>;
    using TexStorage = MaterialTexStorage;

    MaterialStorage();
    virtual ~MaterialStorage();

    using DataStorageCleanup = DataStorage::CleanUp;
    void initDataStorage(GLuint capacity,
                         GLbitfield storage_flags,
                         const DataStorageCleanup& cleanup = std::nullopt);
    DataStorage& getDataStorage();
    const DataStorage& getDataStorage() const;

    using TexStorageCleanup = TexStorage::CleanUp;
    void initTexStorage(GLuint tex_i,
                        GLuint capacity,
                        GLuint levels,
                        GLuint width,
                        GLuint height,
                        GLenum internal_format,
                        const TexStorageCleanup& clean_up = std::nullopt);
    TexStorage& getTexStorage(GLuint tex_i);
    const TexStorage& getTexStorage(GLuint tex_i) const;

private:
    std::unique_ptr<DataStorage> data_storage;
    std::unordered_map<GLuint, std::unique_ptr<TexStorage>> tex_storage;
};

template <IsTriviallyCopyable T>
MaterialStorage<T>::MaterialStorage() {}

template <IsTriviallyCopyable T>
MaterialStorage<T>::~MaterialStorage() {}

template <IsTriviallyCopyable T>
void MaterialStorage<T>::initDataStorage(GLuint capacity,
                                         GLbitfield storage_flags,
                                         const DataStorageCleanup& cleanup) {
    if (data_storage) {
        throw std::logic_error("Already initialized");
    }
    data_storage = std::make_unique<DataStorage>(storage_flags, capacity, cleanup);
}

template <IsTriviallyCopyable T>
void MaterialStorage<T>::initTexStorage(GLuint tex_i,
                                        GLuint capacity,
                                        GLuint levels,
                                        GLuint width,
                                        GLuint height,
                                        GLenum internal_format,
                                        const TexStorageCleanup& clean_up) {

    if (tex_storage.contains(tex_i)) {
        throw std::logic_error("Already initialized");
    }
    tex_storage[tex_i] = std::make_unique<TexStorage>(levels,
                                                      width,
                                                      height,
                                                      capacity,
                                                      internal_format,
                                                      clean_up);
};

template <IsTriviallyCopyable T>
MaterialStorage<T>::DataStorage& MaterialStorage<T>::getDataStorage() {
    if (not data_storage) {
        throw std::logic_error("Is not initialized");
    }
    return *data_storage;
}

template <IsTriviallyCopyable T>
const MaterialStorage<T>::DataStorage& MaterialStorage<T>::getDataStorage() const {
    if (not data_storage) {
        throw std::logic_error("Is not initialized");
    }
    return *data_storage;
}

template <IsTriviallyCopyable T>
MaterialStorage<T>::TexStorage& MaterialStorage<T>::getTexStorage(GLuint tex_i) {
    if (not tex_storage.contains(tex_i)) {
        throw std::logic_error("Is not initialized");
    }
    return *tex_storage.at(tex_i);
}

template <IsTriviallyCopyable T>
const MaterialStorage<T>::TexStorage& MaterialStorage<T>::getTexStorage(GLuint tex_i)
    const {
    if (not tex_storage.contains(tex_i)) {
        throw std::logic_error("Is not initialized");
    }
    return *tex_storage.at(tex_i);
}

} // namespace gnev::base