#pragma once

#include <iostream>

#include "Vertex.hpp"
#include "voxel/type/SolidCube.hpp"

class VoxelTypeTest : public gnev::VoxelTypeSolidCube {
public:
    VoxelTypeTest(int material_id);
    virtual ~VoxelTypeTest();

    virtual gnev::VoxelRectMesh get_rect_mesh(const gnev::VoxelRectInfo& rect,
                                              size_type base_index,
                                              GLenum index_type,
                                              const gnev::data::VertexInfo& vertex_info) override;
};
