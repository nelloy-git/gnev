#pragma once

#include <memory>

namespace gnev::voxel
{

struct Region
{
    using pos_type = float;

    pos_type x = 0;
    pos_type y = 0;
    pos_type z = 0;
    pos_type size_x = 0;
    pos_type size_y = 0;
    pos_type size_z = 0;
};

struct MeshData
{    
    unsigned int indices_count;
    std::shared_ptr<void> indices_data;
    unsigned int vertices_count;
    std::shared_ptr<void> vertices_data;
};
    
} // namespace gnev
