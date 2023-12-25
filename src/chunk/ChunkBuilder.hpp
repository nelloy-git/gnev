#pragma once

#include "chunk/Mesh.hpp"
#include "chunk/Voxel.hpp"

namespace gnev {

class ChunkBuilder {
public:
    ChunkBuilder() = default;
    virtual ~ChunkBuilder() = default;

    virtual Ref<Mesh> getMesh() const = 0;
    virtual void set(GLuint x, GLuint y, GLuint z, const Voxel& voxel) = 0;
    virtual void build() = 0;
};

} // namespace gnev