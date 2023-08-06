#pragma once

#include "data/TriangleMap.hpp"

#include "voxel/Voxel.hpp"

namespace gnev {

template<IsVertex V>
class VoxelChunk {
public: 
    VoxelChunk(const std::shared_ptr<GladGLContext> &ctx, GLuint size_x, GLuint size_y, GLuint size_z);
    virtual ~VoxelChunk();

    void set(const std::shared_ptr<VoxelType>& voxel_type, GLuint x, GLuint y, GLuint z);
    void build_mesh();

private:
    template<typename T>
    using Array1d = std::vector<T>;
    template<typename T>
    using Array2d = std::vector<VoxelArray1d<T>>;
    template<typename T>
    using Array3d = std::vector<VoxelArray2d<T>>;

    TriangleMap<GLuint, V> _triangles;
    VoxelArray3d _voxels;
    GLBufferVectorT<L> _lights;

};

template<IsVertex V>
VoxelChunk<V>::VoxelChunk(const std::shared_ptr<GladGLContext> &ctx,
                          GLuint size_x, GLuint size_y, GLuint size_z)
    : _triangles(ctx),
      _voxels(size_x, VoxelArray2d(size_y, VoxelArray1d(size_z, nullptr))),
      _lights(ctx, nullptr, 0, GL_DYNAMIC_COPY)
{
}

template<IsVertex V>
VoxelChunk<V>::~VoxelChunk()
{
}

template<IsVertex V>
void VoxelChunk<V>::set(const std::shared_ptr<VoxelType>& voxel_type, GLuint x, GLuint y, GLuint z)
{
    _voxels[x][y][z] = std::make_unique<Voxel>(voxel_type);
}

template<IsVertex V>
void VoxelChunk<V>::build_mesh()
{
    

    for (auto& layer : _voxels){
        for (auto& line : layer){
            for (auto)
        }
    }
}

}