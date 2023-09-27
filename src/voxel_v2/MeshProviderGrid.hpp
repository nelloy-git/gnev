#pragma once

#include <optional>
#include <unordered_map>

#include "voxel_v2/MeshProvider.hpp"

namespace gnev::voxel_v2 {

template <data::IsMesh M>
class MeshProviderGrid : public MeshProvider<M> {
public:
    using element = std::shared_ptr<MeshProvider<M>>;

    MeshProviderGrid(std::size_t size_x, std::size_t size_y, std::size_t size_z){};

    std::optional<element> get(std::size_t x, std::size_t y, std::size_t z) const {
        auto x_iter = _map.find(x);
        if (x_iter == _map.end()) {
            return std::nullopt;
        }

        auto y_iter = x_iter->find(y);
        if (y_iter == x_iter->end()) {
            return std::nullopt;
        }

        auto z_iter = y_iter->find(z);
        if (z_iter == y_iter->end()) {
            return std::nullopt;
        }

        return *z_iter;
    };

    void set(const element& value, std::size_t x, std::size_t y, std::size_t z) {
        _map[x][y][z] = value;
    }

    std::vector<M&> getMeshes() const {
        std::vector<M&> list;
        for (auto& x_coord : _map) {
            for (auto& y_coord : x_coord) {
                for (auto& z_coord : y_coord) {
                    if (z_coord) {
                        auto sublist = z_coord->getMeshes();
                        list.push_back(sublist);
                    }
                }
            }
        }
        return list;
    };

private:
    using MeshProvider1D = std::unordered_map<std::size_t, element>;
    using MeshProvider2D = std::unordered_map<std::size_t, MeshProvider1D>;
    using MeshProvider3D = std::unordered_map<std::size_t, MeshProvider2D>;

    MeshProvider3D _map;
};

} // namespace gnev::voxel_v2