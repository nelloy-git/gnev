#pragma once

#include "chunk/Voxel.hpp"
#include "material/base/Material.hpp"
#include "util/Ref.hpp"

namespace gnev {

class SolidBlock {
public:
    SolidBlock(const Ref<Material> material);

    virtual void addV

        private
        : Ref<Material>
              material;
};

} // namespace gnev