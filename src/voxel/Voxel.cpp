#include "Voxel.hpp"

using namespace gnev;

Voxel::Voxel(const std::shared_ptr<const VoxelType>& type)
    : type(type)
{    
}

Voxel::~Voxel()
{
}

bool Voxel::is_transparent_for(VoxelSide side) const
{
    return type->is_transparent_for(side);
}

bool Voxel::can_merge_with(VoxelSide side, const Voxel& other) const
{
    return type->can_merge_with(side, *other.type);
}