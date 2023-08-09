#pragma once

#include "voxel/VoxelSide.hpp"
#include "util/Cube.hpp"

namespace gnev {

class VoxelType;

struct VoxelRectData {
    VoxelSide side;
    std::shared_ptr<VoxelType> type;
};
using VoxelRect = Cube<VoxelRectData>;
    
} // namespace gnev
