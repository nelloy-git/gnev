#pragma once

#include <vector>

#include "data/Mesh.hpp"

namespace gnev::voxel_v2 {

template <data::IsMesh M>
class MeshProvider {
public:
    MeshProvider();
    virtual ~MeshProvider();

    virtual std::vector<M>& getMeshes() const = 0;
};

} // namespace gnev::voxel_v2