#pragma once

#include <memory>

#include "material/base/MaterialFactory.hpp"
#include "material/pbr/MaterialPBR.hpp"
#include "material/pbr/MaterialStoragePBR.hpp"

namespace gnev {

class MaterialFactoryPBR : public base::MaterialFactory<MaterialPBR> {
public:
    std::shared_ptr<MaterialStoragePBR> getStorage();
    std::shared_ptr<const MaterialStoragePBR> getStorage() const;
};

} // namespace gnev