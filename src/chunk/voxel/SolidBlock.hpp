#pragma once

#include "chunk/Voxel.hpp"
#include "material/base/Material.hpp"

namespace gnev {

class SolidBlock {
public:
    SolidBlock(const Ref<base::Material> material);
}

} // namespace gnev