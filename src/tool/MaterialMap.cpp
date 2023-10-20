// #include "tool/MaterialMap.hpp"

// #include <stdexcept>

// namespace gnev::tool {

// MaterialMap::MaterialMap(Index capacity)
//     : data_buffer(capacity), diffuse_tex() {
//     for (Index i = 0; i < capacity; ++i) {
//         unused.insert(i);
//     }
// }

// MaterialMap::~MaterialMap() {}

// MaterialData& MaterialMap::initMaterial() {
//     Index index = extractUnusedIndex();
//     auto& material = this->operator[](index);
//     material = MaterialData();
//     material.indices.self_index = index;
//     return material;
// }

// void MaterialMap::freeMaterial(MaterialData& material) {
//     if (material.getIndex() == RESERVED_INDEX) {
//         throw std::out_of_range("");
//     }

//     unused.insert(material.indices.self_index);
//     material = MaterialData();
// }

// MaterialData& MaterialMap::getMaterial(Index index) {
//     auto& material = this->operator[](index);
//     if (material.getIndex() != index) {
//         throw std::out_of_range("");
//     }
//     return material;
// }

// const MaterialData& MaterialMap::getMaterial(Index index) const {
//     auto& material = this->operator[](index);
//     if (material.getIndex() != index) {
//         throw std::out_of_range("");
//     }
//     return material;
// }

// MaterialData::Index MaterialMap::extractUnusedIndex() {
//     auto iter = unused.begin();
//     if (iter == unused.end()) {
//         throw std::out_of_range("");
//     }

//     Index index = *iter;
//     unused.erase(iter);
//     return index;
// }

// } // namespace gnev::tool