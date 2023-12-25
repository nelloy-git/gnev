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
    virtual void set(GLuint x, GLuint y, GLuint z, const Ptr<Voxel>& voxel);
    virtual void build();

private:
    Array3d<Ptr<Voxel>> voxels;
    Ref<Mesh> active;
    Ref<Mesh> passive;
};

} // namespace gnev