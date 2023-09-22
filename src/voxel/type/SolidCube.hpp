#pragma once

#include "voxel/VoxelType.hpp"

namespace gnev {

class EXPORT VoxelTypeSolidCube : public VoxelType {
public:
    using size_type = unsigned int;
    static constexpr unsigned int index_n = 4;
    static constexpr unsigned int vertex_n = 4;

    using IndexArray = std::array<size_type, 6>;
    using PosArray = std::array<std::array<float, 3>, 4>;
    using UVArray = std::array<std::array<float, 2>, 4>;

    VoxelTypeSolidCube();
    virtual ~VoxelTypeSolidCube();

    int top_material_id;
    int bottom_material_id;
    int front_material_id;
    int back_material_id;
    int left_material_id;
    int right_material_id;

    virtual IndexArray get_indices(const VoxelRectInfo& rect, size_type base_index);
    virtual PosArray get_position(const VoxelRectInfo& rect);
    virtual UVArray get_uv(const VoxelRectInfo& rect);
    virtual int get_material_id(const VoxelRectInfo& rect);
};

} // namespace gnev