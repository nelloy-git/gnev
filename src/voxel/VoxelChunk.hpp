#pragma once

#include "data/TriangleMap.hpp"

#include "voxel/Voxel.hpp"

namespace gnev {

template<IsVertex V, typename L>
class VoxelChunk {
public: 
    VoxelChunk(const std::shared_ptr<GladGLContext> &ctx, GLuint width, GLuint height);
    virtual ~VoxelChunk();

private:
    TriangleMap<GLuint, V> _triangles;
    std::vector<std::vector<std::unique_ptr<Voxel>>> _voxels;
    GLBufferVectorT<L> _lights;

};

template<IsVertex V, typename L>
VoxelChunk<V,L>::VoxelChunk(const std::shared_ptr<GladGLContext> &ctx, GLuint width, GLuint height)
    : _triangles(ctx),
      _voxels(width, std::vector<std::unique_ptr<Voxel>>(height, nullptr)),
      _lights(ctx, nullptr, 0, GL_DYNAMIC_COPY)
{
}

template<IsVertex V, typename L>
VoxelChunk<V,L>::~VoxelChunk()
{
}


}