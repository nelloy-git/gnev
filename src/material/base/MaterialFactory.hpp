#pragma once

#include <initializer_list>

#include "material/base/Material.hpp"
#include "material/base/MaterialStorage.hpp"

namespace gnev::base {

template <IsMaterial M>
class EXPORT MaterialFactory {
public:
    using Material = M;
    using Data = M::Data;
    using Storage = MaterialStorage<Data>;

    struct DataStorageSettings {
        GLuint capacity;
        GLbitfield storage_flags;
        const Storage::DataStorageCleanup& clean_up;
    };

    struct TexStorageSettings {
        GLuint tex_i;
        GLuint capacity;
        GLuint levels;
        GLuint width;
        GLuint height;
        GLenum internal_format;
        const Storage::TexStorageCleanup& clean_up;
    };

    MaterialFactory(const DataStorageSettings& data_settings,
                    std::initializer_list<
                        std::reference_wrapper<const TexStorageSettings>> tex_settings);
    virtual ~MaterialFactory();

    std::shared_ptr<Storage> getStorage();
    std::shared_ptr<const Storage> getStorage() const;

    virtual Material create() = 0;

private:
    std::shared_ptr<Storage> storage;
};

template <IsMaterial M>
MaterialFactory<M>::MaterialFactory(const DataStorageSettings& data_settings,
                                    std::initializer_list<
                                        std::reference_wrapper<const TexStorageSettings>>
                                        tex_settings)
    : storage(std::make_shared<Storage>()) {
    storage->initDataStorage(data_settings.capacity,
                             data_settings.storage_flags,
                             data_settings.clean_up);
    for (auto ref : tex_settings) {
        const TexStorageSettings& cur = ref;
        storage->initTexStorage(cur.tex_i,
                                cur.capacity,
                                cur.levels,
                                cur.width,
                                cur.height,
                                cur.internal_format,
                                cur.clean_up);
    }
}

template <IsMaterial M>
MaterialFactory<M>::~MaterialFactory() {}

template <IsMaterial M>
std::shared_ptr<typename MaterialFactory<M>::Storage> MaterialFactory<M>::getStorage() {
    return storage;
}

template <IsMaterial M>
std::shared_ptr<const typename MaterialFactory<M>::Storage> MaterialFactory<
    M>::getStorage() const {
    return storage;
}

} // namespace gnev::base