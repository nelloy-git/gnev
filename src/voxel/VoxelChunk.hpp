#pragma once

#include "data/Mesh.hpp"
#include "util/CubeContainer.hpp"
#include "voxel/VoxelSideMap.hpp"

namespace gnev {

template <typename I, data::IsVertex V>
class EXPORT VoxelChunk {
public:
    using size_type = unsigned int;

    VoxelChunk(const std::shared_ptr<GladGLContext>& ctx,
               size_type size_x,
               size_type size_y,
               size_type size_z);
    virtual ~VoxelChunk();

    void init(size_type size_x, size_type size_y, size_type size_z);

    data::Mesh<I, V>& mesh();

    std::shared_ptr<const VoxelType> get(GLuint x, GLuint y, GLuint z) const;
    void set(const std::shared_ptr<VoxelType>& voxel_type, GLuint x, GLuint y, GLuint z);
    void apply_mesh();

private:
    data::Mesh<I, V> _mesh;

    Array3d<std::shared_ptr<VoxelType>> _voxels;
    VoxelSideMap _top;
    VoxelSideMap _bottom;
    VoxelSideMap _front;
    VoxelSideMap _back;
    VoxelSideMap _right;
    VoxelSideMap _left;

    void _insert_side_map(size_type& base_index, VoxelSideMap& side_map);
};

template <typename I, data::IsVertex V>
VoxelChunk<I, V>::VoxelChunk(const std::shared_ptr<GladGLContext>& ctx,
                             size_type size_x,
                             size_type size_y,
                             size_type size_z)
    : _mesh(ctx)
    , _voxels(size_x, size_y, size_z, nullptr)
    , _top(VoxelSide::Top)
    , _bottom(VoxelSide::Bottom)
    , _front(VoxelSide::Front)
    , _back(VoxelSide::Back)
    , _right(VoxelSide::Left)
    , _left(VoxelSide::Right) {
    _top.init(size_x, size_y, size_z);
    _bottom.init(size_x, size_y, size_z);
    _front.init(size_x, size_y, size_z);
    _back.init(size_x, size_y, size_z);
    _right.init(size_x, size_y, size_z);
    _left.init(size_x, size_y, size_z);
}

template <typename I, data::IsVertex V>
VoxelChunk<I, V>::~VoxelChunk() {}

template <typename I, data::IsVertex V>
data::Mesh<I, V>& VoxelChunk<I, V>::mesh() {
    return _mesh;
}

template <typename I, data::IsVertex V>
std::shared_ptr<const VoxelType> VoxelChunk<I, V>::get(GLuint x, GLuint y, GLuint z) const {
    return _voxels(x, y, z);
}

template <typename I, data::IsVertex V>
void VoxelChunk<I, V>::set(const std::shared_ptr<VoxelType>& voxel_type,
                           size_type x,
                           size_type y,
                           size_type z) {
    _voxels(x, y, z) = voxel_type;
}

template <typename I, data::IsVertex V>
void VoxelChunk<I, V>::apply_mesh() {
    _mesh.indices().clear();
    _mesh.vertices().clear();

    size_type base_index = 0;
    _insert_side_map(base_index, _top);
    _insert_side_map(base_index, _bottom);
    _insert_side_map(base_index, _front);
    _insert_side_map(base_index, _back);
    _insert_side_map(base_index, _right);
    _insert_side_map(base_index, _left);

    std::cout << "Total indices: " << _mesh.indices().getSize()
              << " vertices: " << _mesh.vertices().getSize() << std::endl;
}

template <typename I, data::IsVertex V>
void VoxelChunk<I, V>::_insert_side_map(size_type& base_index, VoxelSideMap& side_map) {
    side_map.build_map(_voxels);
    side_map.build_rectangles();

    auto& rectangles = side_map.get_rectangle_infos();
    std::cout << "Side: " << static_cast<int>(side_map.side)
              << " fount rectangles: " << rectangles.size() << std::endl;

    size_t n = 0;
    for (auto& rect : rectangles) {
        auto voxel_mesh = rect->type->get_rect_mesh(*rect,
                                                    base_index,
                                                    _mesh.Index,
                                                    decltype(_mesh)::vertex_type::info);
        std::cout << "N: " << ++n << " indices: " << voxel_mesh.indices_count
                  << " vertices: " << voxel_mesh.vertices_count << std::endl;

        if (voxel_mesh.indices_count == 0) {
            continue;
        }

        _mesh.indices().pushRangeBack(voxel_mesh.indices_count,
                                      static_cast<const I*>(voxel_mesh.indices_data.get()));
        _mesh.vertices().pushRangeBack(voxel_mesh.vertices_count,
                                       static_cast<const V*>(voxel_mesh.vertices_data.get()));
        base_index += voxel_mesh.vertices_count;
    }
}

} // namespace gnev