#include "ChunkBased/Container.hpp"

#include "voxel/Chunk.hpp"

using I = unsigned int;
using V = Vertex;

VoxelContainer::VoxelContainer() {}

VoxelContainer::~VoxelContainer() {}

void VoxelContainer::prepare_test_1(const gnev::gl::GladCtx& ctx) {
    auto chunk_1 = std::make_shared<gnev::voxel::Chunk<I, V>>(ctx);
    chunk_1->pos = {0, 0, 0};
    chunk_1->size = {4, 4, 4};
    chunk_1->elements = {4, 4, 4};
}