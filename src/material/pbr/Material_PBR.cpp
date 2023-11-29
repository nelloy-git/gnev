#include "material/pbr/Material_PBR.hpp"

#include <cstddef>
#include <memory>
#include <stdexcept>

#include "material/base/Material.hpp"
#include "material/base/MaterialStorage.hpp"
#include "material/pbr/MaterialGL_PBR.hpp"

namespace gnev {

Material_PBR::Material_PBR(WeakRef<base::MaterialStorage<MaterialGL_PBR>> weak_storage,
                           const MaterialGL_PBR& initial)
    : base::Material<MaterialGL_PBR>(weak_storage, initial) {}

Material_PBR::~Material_PBR() {}

std::optional<base::MaterialTex> Material_PBR::getTexRef(MaterialTexType_PBR type) const {
    return base::Material<MaterialGL_PBR>::getTexRef(toUint(type));
}

void Material_PBR::setTexRef(MaterialTexType_PBR type,
                             std::optional<Ref<base::MaterialTex>> tex_ref) {
    base::Material<MaterialGL_PBR>::setTexRef(toUint(type), tex_ref);
}

void Material_PBR::setTexOffset(MaterialTexType_PBR type, const glm::vec4& value) {
    getDataRef()->setData<glm::vec4>(&value, MaterialGL_PBR::OffsetOfTexOffset(type));
}

glm::vec4 Material_PBR::getTexOffset(MaterialTexType_PBR type) const {
    glm::vec4 result;
    getDataRef()->getData<glm::vec4>(&result, MaterialGL_PBR::OffsetOfTexOffset(type));
    return result;
}

void Material_PBR::setTexMultiplier(MaterialTexType_PBR type, const glm::vec4& value) {
    getDataRef()->setData<glm::vec4>(&value, MaterialGL_PBR::OffsetOfTexMultiplier(type));
}

glm::vec4 Material_PBR::getTexMultiplier(MaterialTexType_PBR type) const {
    glm::vec4 result;
    getDataRef()->getData<glm::vec4>(&result,
                                     MaterialGL_PBR::OffsetOfTexMultiplier(type));
    return result;
}

} // namespace gnev