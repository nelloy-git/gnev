#pragma once

#include <unordered_set>

#include "gl/buffer/CoherentStorage.hpp"
#include "tool/Material.hpp"

namespace gnev::tool {

class MaterialMap : public gl::buffer::CoherentStorage<Material> {
public:
    using Index = Material::Index;
    static constexpr Index RESERVED_INDEX = Material::RESERVED_INDEX;

    MaterialMap(Index capacity);
    virtual ~MaterialMap();

    Material& initMaterial();
    void freeMaterial(Material& material);
    Material& getMaterial(Index index);
    const Material& getMaterial(Index index) const;

private:
    using CoherentStorage<Material>::at;
    using CoherentStorage<Material>::operator[];

    using CoherentStorage<Material>::back;
    using CoherentStorage<Material>::front;

    using CoherentStorage<Material>::begin;
    using CoherentStorage<Material>::end;

    using CoherentStorage<Material>::data;

    std::unordered_set<Index> unused;

    Index extractUnusedIndex();
};

} // namespace gnev::tool