#include "material/pbr/MaterialFactoryPBR.hpp"

#include <memory>
#include <stdexcept>

#include "material/base/MaterialFactory.hpp"
#include "material/pbr/MaterialPBR.hpp"
#include "material/pbr/MaterialStoragePBR.hpp"

namespace gnev {

using Base = base::MaterialFactory<MaterialPBR>;

std::shared_ptr<MaterialStoragePBR> MaterialFactoryPBR::getStorage() {
    return std::dynamic_pointer_cast<MaterialStoragePBR>(Base::getStorage());
}

std::shared_ptr<const MaterialStoragePBR> MaterialFactoryPBR::getStorage() const {
    return std::dynamic_pointer_cast<const MaterialStoragePBR>(Base::getStorage());
}

} // namespace gnev