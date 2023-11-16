#pragma once

#include <functional>
#include <memory>

#include "material/base/MaterialFactory.hpp"
#include "material/pbr/MaterialPBR.hpp"

namespace gnev {

class MaterialFactoryPBR : public base::MaterialFactory<MaterialPBR> {
public:
    MaterialFactoryPBR(const DataStorageSettings& data_settings,
                       std::initializer_list<
                           std::reference_wrapper<const TexStorageSettings>>
                           tex_settings);
    ~MaterialFactoryPBR();

    MaterialPBR create() override;
};

} // namespace gnev