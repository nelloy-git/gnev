#pragma once

#include <vector>

#include "data/Mesh.hpp"
#include "glm/glm.hpp"

namespace gnev::voxel {

template <data::IsIndex I, data::IsVertex V>
class Voxel {
public:
    Voxel(const glm::vec3& pos = {0, 0, 0}, const glm::vec3& size = {1, 1, 1});
    virtual ~Voxel();

    glm::vec3 pos;
    glm::vec3 size;

    virtual std::vector<data::Mesh<I, V>> mesh() = 0;
};

template <data::IsIndex I, data::IsVertex V>
Voxel<I, V>::Voxel(const glm::vec3& pos, const glm::vec3& size)
    : pos(pos),
      size(size) {}

template <data::IsIndex I, data::IsVertex V>
Voxel<I, V>::~Voxel() {}

} // namespace gnev::voxel