#include "material/pbr/MaterialFactoryPBR.hpp"

#include <memory>
#include <stdexcept>

#include "material/base/MaterialFactory.hpp"
#include "material/pbr/MaterialDataPBR.hpp"
#include "material/pbr/MaterialPBR.hpp"

namespace gnev {

using Base = base::MaterialFactory<MaterialPBR>;

MaterialFactoryPBR::MaterialFactoryPBR(const DataStorageSettings& data_settings,
                                       std::initializer_list<std::reference_wrapper<
                                           const TexStorageSettings>> tex_settings)
    : base::MaterialFactory<MaterialPBR>(data_settings, tex_settings) {}

MaterialFactoryPBR::~MaterialFactoryPBR() {}

MaterialPBR MaterialFactoryPBR::create() {
    auto store = getStorage();
    auto data_index = store->getDataStorage()->initUnusedIndex();
    if (not data_index.has_value()) {
        throw std::out_of_range("");
    }
    return MaterialPBR(store, data_index.value());
}

} // namespace gnev