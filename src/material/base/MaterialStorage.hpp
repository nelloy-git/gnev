#pragma once

#include <array>
#include <memory>

#include "material/base/Defines.hpp"
#include "material/base/ImageLoader.hpp"
#include "material/base/Material.hpp"

namespace gnev::base {

template <IsTriviallyCopyable T, MaterialIndex TexN>
class EXPORT MaterialStorage {
public:
    using DataStorage = MaterialDataStorage<T>;
    using TexStorage = MaterialTexStorage;

    MaterialStorage();
    virtual ~MaterialStorage();

    using DataStorageCleanup = DataStorage::CleanUp;
    void initDataStorage(MaterialIndex capacity,
                         GLbitfield storage_flags,
                         const DataStorageCleanup& cleanup = std::nullopt);
    DataStorage& getDataStorage();
    const DataStorage& getDataStorage() const;

    using TexStorageCleanup = TexStorage::CleanUp;
    void initTexStorage(MaterialIndex tex_i,
                        MaterialIndex capacity,
                        GLuint levels,
                        GLuint width,
                        GLuint height,
                        GLenum internal_format,
                        const TexStorageCleanup& clean_up = std::nullopt);
    TexStorage& getTexStorage(MaterialIndex tex_i);
    const TexStorage& getTexStorage(MaterialIndex tex_i) const;

private:
    std::unique_ptr<DataStorage> data_storage;
    std::array<std::unique_ptr<TexStorage>, TexN> tex_storage;
};

template <IsTriviallyCopyable T, unsigned int TexN>
MaterialStorage<T, TexN>::MaterialStorage() {}

template <IsTriviallyCopyable T, unsigned int TexN>
MaterialStorage<T, TexN>::~MaterialStorage() {}

template <IsTriviallyCopyable T, unsigned int TexN>
void MaterialStorage<T, TexN>::initDataStorage(MaterialIndex capacity,
                                               GLbitfield storage_flags,
                                               const DataStorageCleanup& cleanup) {
    if (data_storage) {
        throw std::logic_error("Already inited");
    }
    data_storage = std::make_unique<DataStorage>(storage_flags, capacity, cleanup);
}

template <IsTriviallyCopyable T, unsigned int TexN>
void MaterialStorage<T, TexN>::initTexStorage(MaterialIndex tex_i,
                                              MaterialIndex capacity,
                                              GLuint levels,
                                              GLuint width,
                                              GLuint height,
                                              GLenum internal_format,
                                              const TexStorageCleanup& clean_up) {
    if (tex_i >= TexN) {
        throw std::out_of_range("");
    }
    if (tex_storage[tex_i]) {
        throw std::logic_error("Already inited");
    }
    tex_storage[tex_i] = std::make_unique<TexStorage>(levels,
                                                      width,
                                                      height,
                                                      capacity,
                                                      internal_format,
                                                      clean_up);
};

template <IsTriviallyCopyable T, unsigned int TexN>
MaterialStorage<T, TexN>::DataStorage& MaterialStorage<T, TexN>::getDataStorage() {
    if (not data_storage) {
        throw std::logic_error("Is not initialized");
    }
    return *data_storage;
}

template <IsTriviallyCopyable T, unsigned int TexN>
const MaterialStorage<T, TexN>::DataStorage& MaterialStorage<T, TexN>::getDataStorage()
    const {
    if (not data_storage) {
        throw std::logic_error("Is not initialized");
    }
    return *data_storage;
}

template <IsTriviallyCopyable T, unsigned int TexN>
MaterialStorage<T, TexN>::TexStorage& MaterialStorage<T, TexN>::
    getTexStorage(MaterialIndex tex_i) {
    if (tex_i >= TexN) {
        throw std::out_of_range("");
    }
    if (not tex_storage[tex_i]) {
        throw std::logic_error("Is not initialized");
    }
    return *tex_storage[tex_i];
}

template <IsTriviallyCopyable T, unsigned int TexN>
const MaterialStorage<T, TexN>::TexStorage& MaterialStorage<T, TexN>::
    getTexStorage(MaterialIndex tex_i) const {
    if (tex_i >= TexN) {
        throw std::out_of_range("");
    }
    if (not tex_storage[tex_i]) {
        throw std::logic_error("Is not initialized");
    }
    return *tex_storage[tex_i];
}

} // namespace gnev::base