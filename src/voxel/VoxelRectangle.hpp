#pragma once

#include <memory>

#include "util/Util.hpp"
#include "voxel/VoxelSide.hpp"

namespace gnev {

class VoxelType;

struct EXPORT VoxelRectInfo {
    using size_type = unsigned int;

    std::shared_ptr<VoxelType> type;
    VoxelSide side;

    size_type x = 0;
    size_type y = 0;
    size_type z = 0;
    size_type size_x = 0;
    size_type size_y = 0;
    size_type size_z = 0;
};

struct VoxelRectMesh {
    using size_type = unsigned int;

    size_type indices_count;
    std::shared_ptr<const void> indices_data;
    size_type vertices_count;
    std::shared_ptr<const void> vertices_data;
};

} // namespace gnev
