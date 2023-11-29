#pragma once

#include "material/base/MaterialFactory.hpp"
#include "material/base/MaterialStorage.hpp"
#include "material/pbr/MaterialGL_PBR.hpp"
#include "material/pbr/Material_PBR.hpp"
#include "util/Ref.hpp"

namespace gnev {

class MaterialFactory_PBR : public base::MaterialFactory<Material_PBR> {
public:
    MaterialFactory_PBR(Ref<base::MaterialStorage<MaterialGL_PBR>>);
    ~MaterialFactory_PBR();

    Material_PBR create();
};

} // namespace gnev