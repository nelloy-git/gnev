#pragma once

#include "data/VertexContainer.hpp"
#include "voxel/VoxelSideMap.hpp"

namespace gnev {

class EXPORT VoxelChunk {
public: 
    using size_type = unsigned int;

    VoxelChunk(const std::shared_ptr<GladGLContext> &ctx,
               GLenum index_type, const std::initializer_list<AttribInfo>& vertex_info,
               size_type size_x, size_type size_y, size_type size_z);
    virtual ~VoxelChunk();

    VertexContainer& mesh();

    void set(const std::shared_ptr<VoxelType>& voxel_type, GLuint x, GLuint y, GLuint z);
    void apply_mesh();


private:
    VertexContainer _mesh;

    Array3d<std::shared_ptr<VoxelType>> _voxels;
    VoxelSideMap _top;

};

}