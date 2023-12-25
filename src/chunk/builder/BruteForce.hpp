#pragma once

#include "chunk/ChunkBuilder.hpp"
#include "gl/data/VertexInfo.hpp"

namespace gnev::chunk_builder {

class BruteForce : public ChunkBuilder {
public:
    BruteForce();
    virtual ~BruteForce() = default;

    Ref<Mesh> getMesh() const override;
    void set(GLuint x, GLuint y, GLuint z, const Voxel& voxel) override;
    void build() override;

private:
    Ref<Mesh> active;
    Ref<Mesh> passive;
};
}