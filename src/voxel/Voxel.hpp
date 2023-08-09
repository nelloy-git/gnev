#pragma once

#include <memory>

#include "util/Util.hpp"

namespace gnev {

class VoxelType;

class EXPORT Voxel {
public:
    Voxel(const std::shared_ptr<const VoxelType>& type, size_t x, size_t y, size_t z);
    virtual ~Voxel();

    const std::shared_ptr<const VoxelType> type;
    const size_t x;
    const size_t y;
    const size_t z;

    std::weak_ptr<Voxel> top;
    std::weak_ptr<Voxel> bottom;
    std::weak_ptr<Voxel> front;
    std::weak_ptr<Voxel> back;
    std::weak_ptr<Voxel> right;
    std::weak_ptr<Voxel> left;
};

}