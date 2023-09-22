#pragma once

#include "voxel/Voxel.hpp"

namespace gnev::voxel {

template <IsIndex I, IsVertex V>
class SolidCube : public Voxel<I, V> {
public:
    SolidCube(const glm::vec3& pos = {0, 0, 0}, const glm::vec3& size = {1, 1, 1});
    virtual ~SolidCube();

protected:
    template <std::size_t PosI>
    void fill_positions(V* dst, const glm::vec3& pos, const glm::vec3& size) const;
};

template <IsIndex I, IsVertex V>
SolidCube<I, V>::SolidCube(const glm::vec3& pos, const glm::vec3& size)
    : Voxel<I, V>(pos, size) {}

template <IsIndex I, IsVertex V>
SolidCube<I, V>::~SolidCube() {}

template <IsIndex I, IsVertex V>
template <std::size_t PosI>
void SolidCube<I, V>::fill_positions(V* dst, const glm::vec3& pos, const glm::vec3& size) const {}

} // namespace gnev::voxel
