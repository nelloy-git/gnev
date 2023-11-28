#pragma once

#include "material/base/MaterialData.hpp"
#include "material/base/MaterialStorage.hpp"
#include "material/base/MaterialTex.hpp"
#include "material/pbr/MaterialFactory_PBR.hpp"
#include "material/pbr/MaterialGL_PBR.hpp"
#include "material/pbr/Material_PBR.hpp"

namespace gnev {

using MaterialGL_PBR = MaterialGL_PBR;
using Material_PBR = Material_PBR;
using MaterialFactory_PBR = MaterialFactory_PBR;

using MaterialStorage_PBR = base::MaterialStorage<MaterialGL_PBR>;
using MaterialDataStorage_PBR = base::MaterialDataStorage<MaterialGL_PBR>;
using MaterialTexStorage_PBR = base::MaterialTexStorage;

using MaterialData_PBR = base::MaterialData<MaterialGL_PBR>;
using MaterialTex_PBR = base::MaterialTex;

} // namespace gnev