#include "voxel/VoxelType.hpp"

using namespace gnev;

VoxelType::VoxelType(bool transparent)
    : _trasparent(transparent)
{
}

VoxelType::~VoxelType()
{
}

bool VoxelType::is_transparent_for(VoxelSide side) const
{
    return _trasparent;
}

bool VoxelType::can_merge_with(VoxelSide size, const VoxelType& other) const
{
    return (this == &other);
}