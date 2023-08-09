#include "voxel/Voxel.hpp"

#include "voxel/VoxelType.hpp"

using namespace gnev;

Voxel::Voxel(const std::shared_ptr<const VoxelType>& type, size_t x, size_t y, size_t z)
    : type(type),
      x(x),
      y(y),
      z(z)
{    
}

Voxel::~Voxel()
{
}
