#pragma once

#include <cstddef>
#include <memory>
#include <unordered_set>

#include "gl/buffer/CoherentStorage.hpp"
#include "gl/texture/ImmutableStorage.hpp"
#include "tool/MaterialData.hpp"

namespace gnev::tool {

template<std::size_t TexN>
class MaterialMap {
public:
    using MaterialData = MaterialData<TexN>;
    using Index = MaterialData::Index;
    static constexpr Index RESERVED_INDEX = MaterialData::RESERVED_INDEX;

    MaterialMap(Index capacity){};
    virtual ~MaterialMap();

    void initMaterialDataStorage(Index capacity);
    void initTextureStorage(Index type,
                            Index capacity,
                            std::size_t width,
                            std::size_t height,
                            GLenum internal_format,
                            std::size_t levels);

    std::unique_ptr<MaterialData, std::function<void(MaterialData*)>> createMaterial(){
        auto deleter = 
    };

    MaterialData& initMaterial();
    void freeMaterial(MaterialData& material);
    MaterialData& getMaterial(Index index);
    const MaterialData& getMaterial(Index index) const;
    Index initDiffuseTexture();
    void freeDiffuseTexture(Index index);
    void loadImage(const gl::texture::Image& img);

private:
    std::unique_ptr<gl::buffer::CoherentStorage<MaterialData>> data_buffer;

    std::unique_ptr<gl::texture::ImmutableStorage> diffuse_tex;
    std::unique_ptr<gl::texture::ImmutableStorage> normal_tex;
    std::unique_ptr<gl::texture::ImmutableStorage> specular_tex;

    std::unordered_set<Index> unused;

    Index extractUnusedIndex();
};

} // namespace gnev::tool