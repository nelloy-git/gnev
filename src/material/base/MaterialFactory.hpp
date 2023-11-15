#pragma once

#include <initializer_list>

#include "material/base/Material.hpp"
#include "material/base/MaterialStorage.hpp"

namespace gnev::base {

template <IsMaterial M>
class MaterialFactory {
public:
    using Material = M;
    using Data = M::Data;
    using Storage = MaterialStorage<Data>;

    struct DataStorageSettings {
        GLuint capacity;
        GLbitfield storage_flags;
        const Storage::DataStorageCleanup& cleanup;
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
                    std::initializer_list<TexStorageSettings> tex_settings);
    virtual ~MaterialFactory();

    std::shared_ptr<Storage> getStorage();
    std::shared_ptr<const Storage> getStorage() const;

    Material create();

private:
    std::shared_ptr<Storage> storage;
};

template <IsMaterial M>
MaterialFactory<M>::MaterialFactory(const DataStorageSettings& data_settings,
                                    std::initializer_list<TexStorageSettings>
                                        tex_settings)
    : storage(std::make_shared<Storage>()) {
    storage->initDataStorage(data_settings.capacity,
                             data_settings.storage_flags,
                             data_settings.cleanup);
    for (auto& cur : tex_settings) {
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
std::shared_ptr<const typename MaterialFactory<M>::Storage> MaterialFactory<M>::getStorage() const {
    return storage;
}

template <IsMaterial M>
MaterialFactory<M>::Material MaterialFactory<M>::create(){
    auto store = getStorage();
    auto data_index = store->getDataStorage().initUnusedIndex();
    if (not data_index.has_value()){
        throw std::out_of_range("");
    }
    return MaterialPBR(store, data_index.value());

}

} // namespace gnev::base