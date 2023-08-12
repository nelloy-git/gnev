#include "voxel/VoxelChunk.hpp"

#include <iostream>

#include "Vertex.hpp"

using namespace gnev;

VoxelChunk::VoxelChunk(const std::shared_ptr<GladGLContext> &ctx,
                       GLenum index_type, const VertexInfo& vertex_info,
                       size_type size_x, size_type size_y, size_type size_z)
    : _mesh(ctx, index_type, vertex_info),
      _voxels(size_x, size_y, size_z, nullptr),
      _top_side_map(this, VoxelSide::Top, size_x, size_y, size_z),
      _bottom_side_map(this, VoxelSide::Bottom, size_x, size_y, size_z),
      _front_side_map(this, VoxelSide::Front, size_x, size_y, size_z),
      _back_side_map(this, VoxelSide::Back, size_x, size_y, size_z),
      _right_side_map(this, VoxelSide::Right, size_x, size_y, size_z),
      _left_side_map(this, VoxelSide::Left, size_x, size_y, size_z)
{
}

VoxelChunk::~VoxelChunk()
{
}

VertexContainer& VoxelChunk::mesh()
{
    return _mesh;
}

const std::shared_ptr<const VoxelType>& VoxelChunk::get(GLuint x, GLuint y, GLuint z) const
{
    

    return _voxels(x, y, z);
}

void VoxelChunk::set(const std::shared_ptr<VoxelType>& voxel_type, size_type x, size_type y, size_type z)
{
    _voxels(x, y, z) = voxel_type;
}

void VoxelChunk::apply_mesh()
{
    _mesh.indices().clear();
    _mesh.vertices().clear();

    size_type base_index = 0;
    _insert_side_map(base_index, _top_side_map);
    _insert_side_map(base_index, _bottom_side_map);
    _insert_side_map(base_index, _front_side_map);
    _insert_side_map(base_index, _back_side_map);
    _insert_side_map(base_index, _right_side_map);
    _insert_side_map(base_index, _left_side_map);
}

void VoxelChunk::_insert_side_map(size_type& base_index, VoxelSideMap& side_map)
{
    side_map.build_map(_voxels);
    side_map.build_rectangles();

    auto& rectangles = side_map.get_rectangle_infos();
    std::cout << "Side: " << static_cast<int>(side_map.side) << " rects: " << rectangles.size() << std::endl;

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