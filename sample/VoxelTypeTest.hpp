#pragma once

#include <iostream>

#include "voxel/type/SolidCube.hpp"

#include "Vertex.hpp"

class VoxelTypeTest : public gnev::VoxelTypeSolidCube {
public:
    VoxelTypeTest(int material_id);
    virtual ~VoxelTypeTest();

    virtual gnev::VoxelRectMesh get_rect_mesh(const gnev::VoxelRectInfo& rect, 
                                              size_type base_index,
                                              GLenum index_type,
                                              const std::vector<gnev::AttribInfo>& vertex_info) override;

};
