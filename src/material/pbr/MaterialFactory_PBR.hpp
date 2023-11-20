#pragma once

#include <functional>
#include <memory>

#include "material/base/MaterialFactory.hpp"
#include "material/pbr/MaterialDataStorage_PBR.hpp"
#include "material/pbr/MaterialStorage_PBR.hpp"
#include "material/pbr/MaterialTexStorage_PBR.hpp"
#include "material/pbr/Material_PBR.hpp"

namespace gnev {

class MaterialFactory_PBR : public base::MaterialFactory<Material_PBR> {
public:
    MaterialFactory_PBR(const std::shared_ptr<MaterialStorage_PBR>& storage);
    ~MaterialFactory_PBR();

    Material_PBR create();
};

} // namespace gnev