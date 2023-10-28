#pragma once

#include "gl/buffer/CoherentIndexMap.hpp"
#include "gl/texture/ImmutableIndexMap.hpp"
#include "material/base/ImageLoader.hpp"
#include "material/base/Material.hpp"

namespace gnev::base {

template <IsTriviallyCopyable T, unsigned int TexN>
class EXPORT MaterialStorage {
public:
    using Index = unsigned int;
    using Material = Material<T, TexN>;
    using Data = Material::Data;
    using DataStorage = Material::DataStorage;
    using TextureStorage = Material::TextureStorage;
    using TextureView = Material::TextureView;

    MaterialStorage(const std::shared_ptr<ImageLoader>& loader);
    virtual ~MaterialStorage();

    void initDataStorage(Index capacity,
                         std::variant<std::monostate, T, std::function<T(std::size_t)>>&
                             initializer = std::monostate);

    Material create();
    Index loadTexture(Index tex_n,
                      const std::filesystem::path& path,
                      const ImageInfo& load_info);

private:
    std::shared_ptr<ImageLoader> loader;

    std::shared_ptr<DataStorage> p_data;
    std::shared_ptr<TextureStorage> p_tex;
};

template <IsTriviallyCopyable T, unsigned int TexN>
MaterialStorage<T, TexN>::MaterialStorage(const std::shared_ptr<ImageLoader>& loader)
    : loader(loader) {
    if (!loader) {
        throw std::logic_error("");
    }
}

template <IsTriviallyCopyable T, unsigned int TexN>
MaterialStorage<T, TexN>::~MaterialStorage() {}

template <IsTriviallyCopyable T, unsigned int TexN>
void MaterialStorage<T, TexN>::
    initDataStorage(Index capacity,
                    std::variant<std::monostate, T, std::function<T(std::size_t)>>&
                        initializer) {
    p_data = std::make_shared<DataStorage>(capacity, )
}

template <IsTriviallyCopyable T, unsigned int TexN>
MaterialStorage<T, TexN>::Material MaterialStorage<T, TexN>::create() {
    return Material(p_data, p_tex);
};

} // namespace gnev::base