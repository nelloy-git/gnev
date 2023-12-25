#include "chunk/Chunk.hpp"

namespace gnev {

Chunk::Chunk(GLuint width, GLuint height, GLuint depth, const Voxel& initial)
    : voxels(width, height, depth, nullptr)
    , active(MakeSharable<Mesh>())
    , passive(MakeSharable<Mesh>()) {}

const Ref<Mesh>& Chunk::getMesh() const { return active; }

void Chunk::set(GLuint x, GLuint y, GLuint z, const Ptr<Voxel>& voxel) {
    voxels(x, y, z) = voxel;
}

void Chunk::build() { active.swap(passive); }

} // namespace gnev