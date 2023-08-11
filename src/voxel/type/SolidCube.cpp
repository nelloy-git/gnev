#include "voxel/type/SolidCube.hpp"

#include <iostream>
#include <stdexcept>

using namespace gnev;

VoxelTypeSolidCube::VoxelTypeSolidCube()
{
}

VoxelTypeSolidCube::~VoxelTypeSolidCube()
{
}

VoxelTypeSolidCube::IndexArray VoxelTypeSolidCube::get_indices(const VoxelRectInfo& rect, size_type base_index)
{
    return {
        base_index,
        base_index + 1,
        base_index + 2,
        base_index + 2,
        base_index + 3,
        base_index
    };
}

VoxelTypeSolidCube::PosArray VoxelTypeSolidCube::get_position(const VoxelRectInfo& rect)
{
    std::cout << "get_position: {" << rect.x << ", " << rect.y << ", " << rect.z << "}, "
              << "{" << rect.size_x << ", " << rect.size_y << ", " << rect.size_z << "}"
              << std::endl;

    switch (rect.side){

    case VoxelSide::Top: return {
        std::array<float, 3>{float(rect.x),               float(rect.y)+1, float(rect.z)},
        std::array<float, 3>{float(rect.x),               float(rect.y)+1, float(rect.z + rect.size_z)},
        std::array<float, 3>{float(rect.x + rect.size_x), float(rect.y)+1, float(rect.z + rect.size_z)},
        std::array<float, 3>{float(rect.x + rect.size_x), float(rect.y)+1, float(rect.z)},
    };

    case VoxelSide::Bottom: return {
        std::array<float, 3>{float(rect.x),               float(rect.y), float(rect.z)},
        std::array<float, 3>{float(rect.x + rect.size_x), float(rect.y), float(rect.z)},
        std::array<float, 3>{float(rect.x + rect.size_x), float(rect.y), float(rect.z + rect.size_z)},
        std::array<float, 3>{float(rect.x),               float(rect.y), float(rect.z + rect.size_z)},
    };

    case VoxelSide::Front: return {
        std::array<float, 3>{float(rect.x)+1, float(rect.y),               float(rect.z)},
        std::array<float, 3>{float(rect.x)+1, float(rect.y + rect.size_y), float(rect.z)},
        std::array<float, 3>{float(rect.x)+1, float(rect.y + rect.size_y), float(rect.z + rect.size_z)},
        std::array<float, 3>{float(rect.x)+1, float(rect.y),               float(rect.z + rect.size_z)},
    };

    case VoxelSide::Back: return {
        std::array<float, 3>{float(rect.x), float(rect.y),               float(rect.z + rect.size_z)},
        std::array<float, 3>{float(rect.x), float(rect.y + rect.size_y), float(rect.z + rect.size_z)},
        std::array<float, 3>{float(rect.x), float(rect.y + rect.size_y), float(rect.z)},
        std::array<float, 3>{float(rect.x), float(rect.y),               float(rect.z)},
    };

    case VoxelSide::Right: return {
        std::array<float, 3>{float(rect.x + rect.size_x), float(rect.y),               float(rect.z)},
        std::array<float, 3>{float(rect.x),               float(rect.y),               float(rect.z)},
        std::array<float, 3>{float(rect.x),               float(rect.y + rect.size_y), float(rect.z)},
        std::array<float, 3>{float(rect.x + rect.size_x), float(rect.y + rect.size_y), float(rect.z)},
    };

    case VoxelSide::Left: return {
        std::array<float, 3>{float(rect.x),               float(rect.y),               float(rect.z)+1},
        std::array<float, 3>{float(rect.x + rect.size_x), float(rect.y),               float(rect.z)+1},
        std::array<float, 3>{float(rect.x + rect.size_x), float(rect.y + rect.size_y), float(rect.z)+1},
        std::array<float, 3>{float(rect.x),               float(rect.y + rect.size_y), float(rect.z)+1},
    };

    default:
        throw std::logic_error("");

    };
}

VoxelTypeSolidCube::UVArray VoxelTypeSolidCube::get_uv(const VoxelRectInfo& rect)
{
    float uv_width = rect.size_x != 0 ? rect.size_x : rect.size_y;
    float uv_height = rect.size_y != 0 ? rect.size_y : rect.size_z;

    switch (rect.side)
    {
    case VoxelSide::Top:
        return {
            std::array<float, 2>{0, 0},
            std::array<float, 2>{float(rect.size_z), 0},
            std::array<float, 2>{float(rect.size_z), float(rect.size_x)},
            std::array<float, 2>{0, float(rect.size_x)},
        };
    case VoxelSide::Bottom:
        return {
            std::array<float, 2>{0, 0},
            std::array<float, 2>{0, float(rect.size_x)},
            std::array<float, 2>{float(rect.size_z), float(rect.size_x)},
            std::array<float, 2>{float(rect.size_z), 0},
        };
    case VoxelSide::Front:
    case VoxelSide::Back:
        return {
            std::array<float, 2>{0, 0},
            std::array<float, 2>{0, float(rect.size_y)},
            std::array<float, 2>{float(rect.size_z), float(rect.size_y)},
            std::array<float, 2>{float(rect.size_z), 0},
        };
    case VoxelSide::Left:
    case VoxelSide::Right:
        return {
            std::array<float, 2>{0, 0},
            std::array<float, 2>{float(rect.size_x), 0},
            std::array<float, 2>{float(rect.size_x), float(rect.size_y)},
            std::array<float, 2>{0, float(rect.size_y)},
        };
    
    default:
        throw std::logic_error("");
    }

    // return {
    //     std::array<float, 2>{0, 0},
    //     std::array<float, 2>{0, uv_height},
    //     std::array<float, 2>{uv_width, uv_height},
    //     std::array<float, 2>{uv_width, 0},
    // };
}

int VoxelTypeSolidCube::get_material_id(const VoxelRectInfo& rect)
{
    switch (rect.side){

    case VoxelSide::Top: return top_material_id;
    case VoxelSide::Bottom: return bottom_material_id;
    case VoxelSide::Front: return front_material_id;
    case VoxelSide::Back: return back_material_id;
    case VoxelSide::Right: return right_material_id;
    case VoxelSide::Left: return left_material_id;

    default:
        throw std::logic_error("");

    };
}