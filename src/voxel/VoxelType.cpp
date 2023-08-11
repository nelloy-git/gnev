#include "voxel/VoxelType.hpp"

#include "stdexcept"

using namespace gnev;


VoxelType::VoxelType()
{
}

VoxelType::~VoxelType()
{
}

bool VoxelType::is_visible(VoxelSide side, const std::shared_ptr<const VoxelType>& neighbour) const
{
    if (!neighbour){
        return true;
    }

    return neighbour->is_neighbour_side_visible(_get_opposite(side), *neighbour);
}

bool VoxelType::is_neighbour_side_visible(VoxelSide neighbour_pos, const VoxelType& neighbour) const
{
    return false;
}

VoxelSide VoxelType::_get_opposite(VoxelSide side)
{
    switch (side)
    {
    using enum VoxelSide;
    case Top: return Bottom;
    case Bottom: return Top;
    case Front: return Back;
    case Back: return Front;
    case Left: return Right;
    case Right: return Left;
    
    default:
        throw std::logic_error("");
    }
}