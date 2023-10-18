#pragma once

#include <unordered_set>

#include "gl/buffer/CoherentStorage.hpp"
#include "gl/texture/ImmutableStorage.hpp"
#include "tool/MaterialData.hpp"

namespace gnev::tool {

class MaterialMap {
public:
    using Index = MaterialData::Index;
    static constexpr Index RESERVED_INDEX = MaterialData::RESERVED_INDEX;

    MaterialMap(Index capacity);
    virtual ~MaterialMap();

    MaterialData& initMaterial();
    void freeMaterial(MaterialData& material);
    MaterialData& getMaterial(Index index);
    const MaterialData& getMaterial(Index index) const;

    void initDiffuseStorage(std::size_t width,
                            std::size_t height,
                            GLenum internal_format,
                            std::size_t capacity,
                            std::size_t levels);
    Index initDiffuseTexture();
    void freeDiffuseTexture(Index index);
    void loadImage(const gl::texture::Image& img);

private:
    gl::buffer::CoherentStorage<MaterialData> data_buffer;

    std::unique_ptr<gl::texture::ImmutableStorage> diffuse_tex;
    std::unique_ptr<gl::texture::ImmutableStorage> normal_tex;
    std::unique_ptr<gl::texture::ImmutableStorage> specular_tex;

    std::unordered_set<Index> unused;

    Index extractUnusedIndex();
};

} // namespace gnev::tool