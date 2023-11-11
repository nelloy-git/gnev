#pragma once

#include "material/base/Material.hpp"
#include "material/base/MaterialStorage.hpp"

namespace gnev::base {

template <IsTriviallyCopyable T, MaterialIndex TexN>
class MaterialFactory {
public:
    using Storage = MaterialStorage<T, TexN>;

    struct DataStorageSettings {
        MaterialIndex capacity;
        GLbitfield storage_flags;
        const Storage::DataStorageCleanup& cleanup;
    };

    struct TexStorageSetting {
        MaterialIndex capacity;
        GLuint levels;
        GLuint width;
        GLuint height;
        GLenum internal_format;
        const Storage::TexStorageCleanup& clean_up
    };

    MaterialFactory(const DataStorageSettings& data_settings,
                    const std::array<TexStorageSetting, TexN>& tex_settings);
    virtual ~MaterialFactory();

    Material<T, TexN> create();

private:
    std::shared_ptr<Storage> storage;
};

template <IsTriviallyCopyable T, MaterialIndex TexN>
MaterialFactory<T, TexN>::MaterialFactory(const DataStorageSettings& data_settings,
                                          const std::array<TexStorageSetting, TexN>&
                                              tex_settings)
    : storage(std::make_shared<Storage>()) {
    storage->initDataStorage(data_settings.capacity,
                             data_settings.storage_flags,
                             data_settings.cleanup);
    for (MaterialIndex i = 0; i < tex_settings.size(); ++i) {
        storage->initTexStorage(i,
                                tex_settings[i].capacity,
                                tex_settings[i].levels,
                                tex_settings[i].width,
                                tex_settings[i].height,
                                tex_settings[i].internal_format,
                                tex_settings[i].clean_up);
    }
}

template <IsTriviallyCopyable T, MaterialIndex TexN>
MaterialFactory<T, TexN>::~MaterialFactory() {}

template <IsTriviallyCopyable T, MaterialIndex TexN>
Material<T, TexN> MaterialFactory<T, TexN>::create(){
    
}

} // namespace gnev::base