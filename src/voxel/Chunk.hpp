#pragma once

#include <unordered_map>

#include "voxel/Voxel.hpp"

namespace gnev::voxel {

template <data::IsIndex I, data::IsVertex V>
class MeshBuilder;

template <data::IsIndex I, data::IsVertex V>
class EXPORT Chunk : public Voxel<I, V> {
public:
    Chunk(const gl::GladCtx& ctx);
    virtual ~Chunk();

    glm::vec<3, std::size_t> elements;

    std::vector<data::Mesh<I, V>> mesh() override;

private:
    using Cell = std::unordered_map<
        std::size_t,
        std::unordered_map<std::size_t,
                           std::unordered_map<std::size_t, std::shared_ptr<Voxel<I, V>>>>>;

    data::Mesh<I, V> _active_mesh;

    std::shared_ptr<Cell> _voxels;
    std::shared_ptr<MeshBuilder<I, V>> _builder;
};

template <data::IsIndex I, data::IsVertex V>
Chunk<I, V>::Chunk(const gl::GladCtx& ctx)
    : Voxel<I, V>()
    , _active_mesh(ctx) {}

template <data::IsIndex I, data::IsVertex V>
Chunk<I, V>::~Chunk() {}

template <data::IsIndex I, data::IsVertex V>
std::vector<data::Mesh<I, V>> Chunk<I, V>::mesh() {
    return {_active_mesh};
}

} // namespace gnev::voxel