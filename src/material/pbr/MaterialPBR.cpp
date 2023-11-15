#include "material/pbr/MaterialPBR.hpp"

#include <memory>
#include <stdexcept>

#include "material/base/Material.hpp"
#include "material/pbr/MaterialDataPBR.hpp"
#include "material/pbr/MaterialStoragePBR.hpp"

namespace gnev {

namespace {

inline unsigned int toUint(ParamPBR p) { return static_cast<unsigned int>(p); }

} // namespace

MaterialPBR::MaterialPBR(const std::shared_ptr<MaterialStoragePBR>& storage,
                         GLuint data_index)
    : base::Material<
          MaterialDataPBR>(std::static_pointer_cast<MaterialStoragePBR::Base>(storage),
                           data_index) {}

MaterialPBR::~MaterialPBR() {}

std::shared_ptr<MaterialStoragePBR> MaterialPBR::getStorage() const {
    auto store =
        std::dynamic_pointer_cast<Storage>(base::Material<MaterialDataPBR>::getStorage());
    if (!store) {
        throw std::runtime_error("");
    }
    return store;
}

void MaterialPBR::setTextureIndex(ParamPBR param, GLuint value) {
    auto store = getStorage();
    auto data = store->getDataStorage()[getDataIndex()];
    data->tex_index[toUint(param)] = value;
}

GLuint MaterialPBR::getTextureIndex(ParamPBR param) const {
    auto store = getStorage();
    auto data = store->getDataStorage()[getDataIndex()];
    return data->tex_index[toUint(param)];
}

void MaterialPBR::setTextureOffset(ParamPBR param, const glm::vec4& value) {
    auto store = getStorage();
    auto data = store->getDataStorage()[getDataIndex()];
    data->tex_offset[toUint(param)] = value;
}

glm::vec4 MaterialPBR::getTextureOffset(ParamPBR param) const {
    auto store = getStorage();
    auto data = store->getDataStorage()[getDataIndex()];
    return data->tex_offset[toUint(param)];
}

void MaterialPBR::setTextureMultiplier(ParamPBR param, const glm::vec4& value) {
    auto store = getStorage();
    auto data = store->getDataStorage()[getDataIndex()];
    data->tex_multiplier[toUint(param)] = value;
}

glm::vec4 MaterialPBR::getTextureMultiplier(ParamPBR param) const {
    auto store = getStorage();
    auto data = store->getDataStorage()[getDataIndex()];
    return data->tex_multiplier[toUint(param)];
}

} // namespace gnev