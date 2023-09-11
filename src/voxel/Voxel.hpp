#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "data/Mesh.hpp"

namespace gnev::voxel {

template<IsIndex I, IsVertex V>
class Voxel {
public:
    Voxel(const glm::vec3& pos = {0, 0, 0}, const glm::vec3& size = {1, 1, 1});
    virtual ~Voxel();

    glm::vec3 pos;
    glm::vec3 size;

    virtual std::vector<Mesh<I,V>>& mesh() = 0;

};

template<IsIndex I, IsVertex V>
Voxel<I,V>::Voxel(const glm::vec3& pos, const glm::vec3& size)
    : pos(pos),
      size(size)
{
}

template<IsIndex I, IsVertex V>
Voxel<I,V>::~Voxel()
{
}

}