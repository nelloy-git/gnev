#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <unordered_set>

#include "gl/buffer/CoherentIndexMap.hpp"
#include "gl/texture/ImmutableStorage.hpp"
#include "tool/MaterialData.hpp"

namespace gnev::tool {

template <std::size_t TexN>
class MaterialMap {
public:
    using MaterialData = MaterialData<TexN>;
    using Index = MaterialData::Index;
    static constexpr Index RESERVED_INDEX = MaterialData::RESERVED_INDEX;

    using MaterialDataPtr = std::unique_ptr<MaterialData, std::function<void(MaterialData*)>>;

    MaterialMap(Index capacity){};
    virtual ~MaterialMap();

    void initMaterialDataStorage(Index capacity);
    void initTextureStorage(Index type,
                            Index capacity,
                            std::size_t width,
                            std::size_t height,
                            GLenum internal_format,
                            std::size_t levels);

    MaterialDataPtr createMaterial() {
        auto del = [buffer = data_buffer](MaterialData* ptr) {
            buffer->freeIndex(ptr->getIndex());
        };
        MaterialData* data = &data_buffer[data_buffer.initUnusedIndex()];
        return std::unique_ptr<MaterialData>(data, del);
    }

    MaterialData& initMaterial();
    void freeMaterial(MaterialData& material);
    MaterialData& getMaterial(Index index);
    const MaterialData& getMaterial(Index index) const;
    Index initDiffuseTexture();
    void freeDiffuseTexture(Index index);
    void loadImage(const gl::texture::Image& img);

private:
    std::shared_ptr<gl::buffer::CoherentIndexMap<MaterialData>> data_buffer;

    std::shared_ptr<gl::texture::ImmutableStorage> diffuse_tex;
    std::shared_ptr<gl::texture::ImmutableStorage> normal_tex;
    std::shared_ptr<gl::texture::ImmutableStorage> specular_tex;

    std::unordered_set<Index> unused;

    Index extractUnusedIndex();
};

} // namespace gnev::tool