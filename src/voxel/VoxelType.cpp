#include "voxel/VoxelType.hpp"

using namespace gnev;


VoxelType::VoxelType()
{
}

VoxelType::~VoxelType()
{
}

bool VoxelType::is_visible(VoxelSide side, const VoxelType& neighbour) const
{
    return true;
}