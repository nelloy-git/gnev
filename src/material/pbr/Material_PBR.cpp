#include "material/pbr/Material_PBR.hpp"

#include <stdexcept>

namespace gnev {

using DataView = Material_PBR::DataView;
using TexView = Material_PBR::TexView;

using DataElem = Material_PBR::DataElem;
using TexElem = Material_PBR::TexElem;

Material_PBR::Material_PBR(WeakRef<DataView> weak_storage, const MaterialGL_PBR& initial)
    : base::Material<MaterialGL_PBR>(weak_storage, initial) {}

MaterialGL_PBR Material_PBR::get() const {
    MaterialGL_PBR result;
    data->get(result);
    return result;
}

void Material_PBR::setTex(MaterialTexType_PBR type, Ptr<TexElem> tex) {
    base::Material<MaterialGL_PBR>::setTex(toUint(type), tex);
}

Ptr<TexElem> Material_PBR::getTex(MaterialTexType_PBR type) const {
    return base::Material<MaterialGL_PBR>::getTex(toUint(type));
}

void Material_PBR::setTexOffset(MaterialTexType_PBR type, const glm::vec4& value) {
    data->set(value, MaterialGL_PBR::OffsetOfTexOffset(type));
}

glm::vec4 Material_PBR::getTexOffset(MaterialTexType_PBR type) const {
    glm::vec4 result;
    data->get(result, MaterialGL_PBR::OffsetOfTexOffset(type));
    return result;
}

void Material_PBR::changeTexOffset(MaterialTexType_PBR type,
                                   const Changer<glm::vec4>& changer) {
    data->change(changer, MaterialGL_PBR::OffsetOfTexOffset(type));
}

void Material_PBR::setTexMultiplier(MaterialTexType_PBR type, const glm::vec4& value) {
    data->set(value, MaterialGL_PBR::OffsetOfTexMultiplier(type));
}

glm::vec4 Material_PBR::getTexMultiplier(MaterialTexType_PBR type) const {
    glm::vec4 result;
    data->get(result, MaterialGL_PBR::OffsetOfTexMultiplier(type));
    return result;
}

void Material_PBR::changeTexMultiplier(MaterialTexType_PBR type,
                                       const Changer<glm::vec4>& changer) {
    data->change(changer, MaterialGL_PBR::OffsetOfTexMultiplier(type));
}

} // namespace gnev