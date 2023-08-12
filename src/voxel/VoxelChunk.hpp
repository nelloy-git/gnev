#pragma once

#include "data/VertexContainer.hpp"
#include "voxel/VoxelSideMap.hpp"

namespace gnev {

class EXPORT VoxelChunk {
public: 
    using size_type = unsigned int;

    VoxelChunk(const std::shared_ptr<GladGLContext> &ctx,
               GLenum index_type, const VertexInfo& vertex_info,
               size_type size_x, size_type size_y, size_type size_z);
    virtual ~VoxelChunk();

    VertexContainer& mesh();

    const std::shared_ptr<const VoxelType>& get(GLuint x, GLuint y, GLuint z) const;
    void set(const std::shared_ptr<VoxelType>& voxel_type, GLuint x, GLuint y, GLuint z);
    void apply_mesh();

private:
    VertexContainer _mesh;

    Array3d<std::shared_ptr<VoxelType>> _voxels;
    VoxelSideMap _top_side_map;
    VoxelSideMap _bottom_side_map;
    VoxelSideMap _front_side_map;
    VoxelSideMap _back_side_map;
    VoxelSideMap _right_side_map;
    VoxelSideMap _left_side_map;

    void _insert_side_map(size_type& base_index, VoxelSideMap& side_map);
};

}