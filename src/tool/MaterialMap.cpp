#include "tool/MaterialMap.hpp"

#include <stdexcept>

#include "tool/Material.hpp"

namespace gnev::tool {

MaterialMap::MaterialMap(Index capacity)
    : gl::buffer::CoherentStorage<Material>(capacity) {
    for (Index i = 0; i < capacity; ++i) {
        unused.insert(i);
    }
}

MaterialMap::~MaterialMap() {}

Material& MaterialMap::initMaterial() {
    Index index = extractUnusedIndex();
    auto& material = this->operator[](index);
    material = Material();
    material.indices.self_index = index;
    return material;
}

void MaterialMap::freeMaterial(Material& material) {
    if (material.getIndex() == RESERVED_INDEX){
        throw std::out_of_range("");
    }

    unused.insert(material.indices.self_index);
    material = Material();
}

Material& MaterialMap::getMaterial(Index index){
    auto& material = this->operator[](index);
    if (material.getIndex() != index){
        throw std::out_of_range("");
    }
    return material;
}

const Material& MaterialMap::getMaterial(Index index) const {
    auto& material = this->operator[](index);
    if (material.getIndex() != index){
        throw std::out_of_range("");
    }
    return material;
}

Material::Index MaterialMap::extractUnusedIndex() {
    auto iter = unused.begin();
    if (iter == unused.end()) {
        throw std::out_of_range("");
    }

    Index index = *iter;
    unused.erase(iter);
    return index;
}

} // namespace gnev::tool