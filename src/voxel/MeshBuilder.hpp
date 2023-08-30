#pragma once

#include <future>

#include "voxel/Chunk.hpp"

namespace gnev::voxel {

template<IsIndex I, IsVertex V>
class EXPORT MeshBuilder {
public:
    MeshBuilder();
    virtual ~MeshBuilder();

    virtual std::pair<std::promise<void>, Mesh<I,V>> build(const std::shared_ptr<Chunk>& chunk) const = 0;


};

template<IsIndex I, IsVertex V>
MeshBuilder<I,V>::MeshBuilder()
{
}

template<IsIndex I, IsVertex V>
inline MeshBuilder<I,V>::~MeshBuilder()
{
}

}