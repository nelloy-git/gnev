#pragma once

#include "material/pbr/MaterialDataPBR.hpp"
#include "material/base/MaterialStorage.hpp"

namespace gnev {

class EXPORT MaterialStoragePBR : public base::MaterialStorage<MaterialDataPBR> {
public:
    using Base = base::MaterialStorage<MaterialDataPBR>;
};

} // namespace gnev