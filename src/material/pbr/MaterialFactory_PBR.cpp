#include "material/pbr/MaterialFactory_PBR.hpp"

#include "material/base/MaterialFactory.hpp"
#include "material/base/MaterialStorage.hpp"
#include "material/pbr/MaterialGL_PBR.hpp"
#include "material/pbr/Material_PBR.hpp"
#include "util/Ref.hpp"

namespace gnev {

MaterialFactory_PBR::MaterialFactory_PBR(Ref<base::MaterialStorage<MaterialGL_PBR>>
                                             storage)
    : base::MaterialFactory<Material_PBR>(storage) {}

MaterialFactory_PBR::~MaterialFactory_PBR(){};

Material_PBR MaterialFactory_PBR::create() { return Material_PBR{getStorage()}; }

} // namespace gnev