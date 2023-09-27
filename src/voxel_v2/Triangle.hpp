#pragma once

#include "data/Mesh.hpp"

namespace gnev::voxel_v2 {

template <data::IsMesh M>
class Chunk;

template <data::IsMesh M>
class Voxel;

template <data::IsMesh M>
class Triangle {
public:
    Triangle(Chunk<M>& owner)
        : owner(owner){};

    virtual ~Triangle(){};

    Chunk<M>& owner;

private:
    std::vector<Voxel<M>&> voxelList;
};

} // namespace gnev::voxel_v2
