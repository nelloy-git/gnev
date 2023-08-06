#pragma once

#include <memory>

#include "voxel/VoxelType.hpp"

namespace gnev {

class Voxel {
public:
    Voxel(const std::shared_ptr<const VoxelType>& type);
    virtual ~Voxel();

    const std::shared_ptr<const VoxelType> type;
    virtual bool is_transparent_for(VoxelSide side) const;
    virtual bool can_merge_with(VoxelSide side, const Voxel& other) const;
};

}