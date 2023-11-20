#include "material/pbr/MaterialFactory_PBR.hpp"

#include <memory>
#include <stdexcept>

#include "material/base/MaterialFactory.hpp"
#include "material/pbr/MaterialStorage_PBR.hpp"
#include "material/pbr/Material_PBR.hpp"

namespace gnev {

using Base = base::MaterialFactory<Material_PBR>;

MaterialFactory_PBR::MaterialFactory_PBR(const std::shared_ptr<MaterialStorage_PBR>&
                                           storage)
    : base::MaterialFactory<Material_PBR>(storage) {}

MaterialFactory_PBR::~MaterialFactory_PBR() {}

Material_PBR MaterialFactory_PBR::create() {
    auto store = std::dynamic_pointer_cast<MaterialStorage_PBR>(storage);
    if (not store) {
        throw std::runtime_error("");
    }
    return Material_PBR(store);
}

} // namespace gnev