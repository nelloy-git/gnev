#pragma once

#include <memory>

#include "voxel/VoxelRect.hpp"
#include "util/Cube.hpp"
#include "util/Util.hpp"

namespace gnev {

class EXPORT VoxelType {
public:
    using size_type = unsigned int;

    VoxelType();
    virtual ~VoxelType();

    struct RectVertices {
        size_type indices_count;
        unsigned int indices_type; // GLenum
        std::shared_ptr<const void> indices_data;
        size_type vertices_count;
        size_type vertex_size;
        std::shared_ptr<const void> vertices_data;
    };
    virtual RectVertices get_rect_mesh(const VoxelRect& rectangle) const = 0;

    virtual bool is_visible(VoxelSide side, const VoxelType& neighbour) const;

private:
};




}