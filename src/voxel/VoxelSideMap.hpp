#pragma once

#include <memory>
#include <optional>

#include "util/Array3d.hpp"
#include "util/Util.hpp"
#include "voxel/VoxelType.hpp"

namespace gnev {

class EXPORT VoxelSideMap {
public:
    using size_type = unsigned int;

    VoxelSideMap(VoxelSide side);
    virtual ~VoxelSideMap();

    void init(size_type size_x,
              size_type size_y,
              size_type size_z,
              const std::shared_ptr<VoxelType>& initial_type = nullptr);

    const VoxelSide side;
    const Array3d<std::shared_ptr<VoxelType>>& get_map() const;
    const std::vector<std::shared_ptr<VoxelRectInfo>>& get_rectangle_infos() const;

    void build_map(const Array3d<std::shared_ptr<VoxelType>>& voxels);
    void build_rectangles();

private:
    Array3d<std::shared_ptr<VoxelType>> _map;
    std::vector<std::shared_ptr<VoxelRectInfo>> _rectangle_infos;

    bool _is_visible(const Array3d<std::shared_ptr<VoxelType>>& voxels,
                     const std::shared_ptr<const VoxelType>& type,
                     size_type x,
                     size_type y,
                     size_type z) const;
    void _search_rect_width(Array3d<uint8_t>& passed, VoxelRectInfo& rect) const;
    void _search_rect_height(Array3d<uint8_t>& passed, VoxelRectInfo& rect) const;
};

} // namespace gnev