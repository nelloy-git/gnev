#pragma once

#include "data/Mesh.hpp"
#include "voxel_v2/Triangle.hpp"

namespace gnev::voxel_v2 {

template <data::IsMesh M>
class Chunk;

template <data::IsMesh M>
class Voxel {
public:
    Voxel(Chunk<M>& owner)
        : owner(owner) {}

    virtual ~Voxel(){};

    Chunk<M>& owner;

private:
    std::vector<Triangle<M>> triangleList;
};

} // namespace gnev::voxel_v2
