#pragma once

#include <vector>

#include "voxel/Voxel.hpp"

#include "Vertex.hpp"

using Voxel = gnev::voxel::Voxel<unsigned int, Vertex>;

class VoxelContainer {
public:
    VoxelContainer();
    virtual ~VoxelContainer();

    void prepare_test_1(const gnev::gl::GladCtx& ctx);
    void draw_test_1();

    std::vector<std::shared_ptr<Voxel>> list;

};