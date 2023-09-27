#pragma once

#include "voxel_v2/MeshProvider.hpp"
#include "voxel_v2/Triangle.hpp"
#include "voxel_v2/Voxel.hpp"

namespace gnev::voxel_v2 {

template<data::IsMesh M>
class Chunk : public MeshProvider<M> {
public:
    Chunk(std::size_t size_x, std::size_t size_y, std::size_t size_z);
    virtual ~Chunk();

    std::vector<M>& getMeshes() const;

private:
    std::vector<Voxel>


};

}