#include "material/pbr/Material_PBR.hpp"

#include <stdexcept>

#include "material/pbr/MaterialStorage_PBR.hpp"

namespace gnev {

using DataGL = Material_PBR::DataGL;
using DataView = Material_PBR::DataView;
using TexView = Material_PBR::TexView;

using DataElem = Material_PBR::DataElem;
using TexElem = Material_PBR::TexElem;

Material_PBR::Material_PBR(const Ref<MaterialStorage_PBR>& material_storage)
    : weak_storage(material_storage)
    , data(MakeSharable<DataElem>(material_storage->getDataView(), DataGL{})) {}

WeakRef<MaterialStorage_PBR> Material_PBR::getWeakStorage() const { return weak_storage; }

Ref<MaterialStorage_PBR> Material_PBR::getStorage() const {
    auto storage_opt = weak_storage.lock();
    if (not storage_opt.has_value()){
        throw std::runtime_error("");
    }
    return storage_opt.value();
}

Ref<DataElem> Material_PBR::getData() const { return data; }

void Material_PBR::setTex(MaterialTexType_PBR type, Ptr<TexElem> tex) {
    this->tex.at(toUint(type)) = tex;
    GLuint tex_index = tex ? tex->getIndex() : DataGL::InvalidIndex;
    data->set(tex_index, DataGL::getOffsetOfTexIndex(type));
}

Ptr<TexElem> Material_PBR::getTex(MaterialTexType_PBR type) const {
    return tex.at(toUint(type));
}

void Material_PBR::setTexOffset(MaterialTexType_PBR type, const glm::vec4& value) {
    data->set(value, DataGL::getOffsetOfColorOffset(type));
}

glm::vec4 Material_PBR::getTexOffset(MaterialTexType_PBR type) const {
    glm::vec4 result;
    data->get(result, DataGL::getOffsetOfColorOffset(type));
    return result;
}

void Material_PBR::setTexMultiplier(MaterialTexType_PBR type, const glm::vec4& value) {
    data->set(value, DataGL::getOffsetOfColorMult(type));
}

glm::vec4 Material_PBR::getTexMultiplier(MaterialTexType_PBR type) const {
    glm::vec4 result;
    data->get(result, DataGL::getOffsetOfColorMult(type));
    return result;
}

} // namespace gnev