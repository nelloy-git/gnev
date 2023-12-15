#pragma once

// #include "chunk/ChunkMeshBuilder.hpp"
#include "chunk/Mesh.hpp"
#include "chunk/Voxel.hpp"
#include "util/Array3d.hpp"

namespace gnev {

class Chunk {
public:
    Chunk(GLuint width, GLuint height, GLuint depth, const Voxel& initial);
    virtual ~Chunk() = default;

    const Ref<Mesh>& getMesh() const;

    void set(GLuint x, GLuint y, GLuint z, const Voxel& voxel);
    const Voxel& get(GLuint x, GLuint y, GLuint z) const;

private:
    // Array3d<std::optional<Voxel>> voxels;
    // Ref<ChunkBuilder> builder;
    // Ref<Mesh> mesh;
};

} // namespace gnev