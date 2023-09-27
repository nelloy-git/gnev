#pragma once

#include <future>

#include "voxel/Chunk.hpp"

namespace gnev::voxel {

template <data::IsIndex I, data::IsVertex V>
class EXPORT MeshBuilder {
public:
    MeshBuilder();
    virtual ~MeshBuilder();

    virtual std::pair<std::promise<void>, Mesh<I, V>>
    build(const std::shared_ptr<Chunk>& chunk) const = 0;
};

template <data::IsIndex I, data::IsVertex V>
MeshBuilder<I, V>::MeshBuilder() {}

template <data::IsIndex I, data::IsVertex V>
inline MeshBuilder<I, V>::~MeshBuilder() {}

} // namespace gnev::voxel