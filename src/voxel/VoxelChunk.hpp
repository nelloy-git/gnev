#pragma once

#include "data/Mesh.hpp"
#include "voxel/VoxelSideMap.hpp"
#include "util/CubeContainer.hpp"

namespace gnev {

template<typename I, IsVertex V>
class EXPORT VoxelChunk {
public: 
    using size_type = unsigned int;

    VoxelChunk(const std::shared_ptr<GladGLContext> &ctx,
               size_type size_x, size_type size_y, size_type size_z);
    virtual ~VoxelChunk();

    void init(size_type size_x, size_type size_y, size_type size_z);

    Mesh<I,V>& mesh();

    std::shared_ptr<const VoxelType> get(GLuint x, GLuint y, GLuint z) const;
    void set(const std::shared_ptr<VoxelType>& voxel_type, GLuint x, GLuint y, GLuint z);
    void apply_mesh();

private:
    Mesh<I,V> _mesh;

    Array3d<std::shared_ptr<VoxelType>> _voxels;
    CubeContainer<VoxelSideMap> _side_map;

    void _insert_side_map(size_type& base_index, VoxelSideMap& side_map);
};



template<typename I, IsVertex V>
VoxelChunk<I, V>::VoxelChunk(const std::shared_ptr<GladGLContext> &ctx,
                             size_type size_x, size_type size_y, size_type size_z)
    : _mesh(ctx),
      _voxels(size_x, size_y, size_z, nullptr),
      _side_map(std::make_tuple(this, VoxelSide::Top),
                std::make_tuple(this, VoxelSide::Bottom),
                std::make_tuple(this, VoxelSide::Front),
                std::make_tuple(this, VoxelSide::Back),
                std::make_tuple(this, VoxelSide::Left),
                std::make_tuple(this, VoxelSide::Right))
{
    _side_map[CubeSide::Top].init(size_x, size_y, size_z);
    _side_map[CubeSide::Bottom].init(size_x, size_y, size_z);
    _side_map[CubeSide::Front].init(size_x, size_y, size_z);
    _side_map[CubeSide::Back].init(size_x, size_y, size_z);
    _side_map[CubeSide::Left].init(size_x, size_y, size_z);
    _side_map[CubeSide::Right].init(size_x, size_y, size_z);
}

template<typename I, IsVertex V>
VoxelChunk<I, V>::~VoxelChunk()
{
}

template<typename I, IsVertex V>
Mesh<I,V>& VoxelChunk<I,V>::mesh()
{
    return _mesh;
}

template<typename I, IsVertex V>
std::shared_ptr<const VoxelType> VoxelChunk<I,V>::get(GLuint x, GLuint y, GLuint z) const
{
    return _voxels(x, y, z);
}

template<typename I, IsVertex V>
void VoxelChunk<I,V>::set(const std::shared_ptr<VoxelType>& voxel_type, size_type x, size_type y, size_type z)
{
    _voxels(x, y, z) = voxel_type;
}

template<typename I, IsVertex V>
void VoxelChunk<I,V>::apply_mesh()
{
    _mesh.indices().clear();
    _mesh.vertices().clear();

    size_type base_index = 0;
    _insert_side_map(base_index, _side_map[CubeSide::Top]);
    _insert_side_map(base_index, _side_map[CubeSide::Bottom]);
    _insert_side_map(base_index, _side_map[CubeSide::Front]);
    _insert_side_map(base_index, _side_map[CubeSide::Back]);
    _insert_side_map(base_index, _side_map[CubeSide::Left]);
    _insert_side_map(base_index, _side_map[CubeSide::Right]);
}

template<typename I, IsVertex V>
void VoxelChunk<I,V>::_insert_side_map(size_type& base_index, VoxelSideMap& side_map)
{
    side_map.build_map(_voxels);
    side_map.build_rectangles();

    auto& rectangles = side_map.get_rectangle_infos();
    for (auto& rect : rectangles){
        auto voxel_mesh = rect->type->get_rect_mesh(*rect, base_index, _mesh.index_type, _mesh.vertex_info);

        if (voxel_mesh.indices_count == 0){
            continue;
        }

        _mesh.indices().push_back(voxel_mesh.indices_count * _mesh.index_size, voxel_mesh.indices_data.get());
        _mesh.vertices().push_back(voxel_mesh.vertices_count * _mesh.vertex_info.size, voxel_mesh.vertices_data.get());
        base_index += voxel_mesh.vertices_count;
    }
}

}