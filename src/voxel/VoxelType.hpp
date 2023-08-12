#pragma once

#include <memory>

#include "data/VertexInfo.hpp"
#include "voxel/VoxelRectangle.hpp"

namespace gnev {

class EXPORT VoxelType {
public:
    using size_type = unsigned int;

    VoxelType();
    virtual ~VoxelType();

    virtual VoxelRectMesh get_rect_mesh(const VoxelRectInfo& info, 
                                        size_type base_index,
                                        GLenum index_type,
                                        const VertexInfo& vertex_info) = 0;

    virtual bool is_visible(VoxelSide side, const std::shared_ptr<const VoxelType>& neighbour) const;
    virtual bool is_neighbour_side_visible(VoxelSide neighbour_pos, const VoxelType& neighbour) const;

private:
    static VoxelSide _get_opposite(VoxelSide side);

};




}