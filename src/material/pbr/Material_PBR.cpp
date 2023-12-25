#include "material/pbr/Material_PBR.hpp"

#include <stdexcept>

namespace gnev {

using DataView = Material_PBR::DataView;
using TexView = Material_PBR::TexView;

using DataElem = Material_PBR::DataElem;
using TexElem = Material_PBR::TexElem;

Material_PBR::Material_PBR(WeakRef<DataView> weak_storage,
                           const MaterialGLdata_PBR& initial)
    : data(MakeSharable<DataElem>(weak_storage, initial)) {}

GLuint Material_PBR::getIndex() const { return data->getIndex(); }

MaterialGLdata_PBR Material_PBR::getGLdata() const {
    MaterialGLdata_PBR result;
    data->get(result);
    return result;
}

void Material_PBR::setTex(MaterialTexType_PBR type, Ptr<TexElem> tex) {
    textures[toUint(type)] = tex;
    GLuint tex_index = tex ? tex->getIndex() : GLdata::InvalidIndex;
    data->set(tex_index, MaterialGLdata_PBR::OffsetOfTexIndex(type));
}

Ptr<TexElem> Material_PBR::getTex(MaterialTexType_PBR type) const {
    return textures[toUint(type)];
}

void Material_PBR::setTexOffset(MaterialTexType_PBR type, const glm::vec4& value) {
    data->set(value, MaterialGLdata_PBR::OffsetOfTexOffset(type));
}

glm::vec4 Material_PBR::getTexOffset(MaterialTexType_PBR type) const {
    glm::vec4 result;
    data->get(result, MaterialGLdata_PBR::OffsetOfTexOffset(type));
    return result;
}

void Material_PBR::changeTexOffset(MaterialTexType_PBR type,
                                   const Changer<glm::vec4>& changer) {
    data->change(changer, MaterialGLdata_PBR::OffsetOfTexOffset(type));
}

void Material_PBR::setTexMultiplier(MaterialTexType_PBR type, const glm::vec4& value) {
    data->set(value, MaterialGLdata_PBR::OffsetOfTexMultiplier(type));
}

glm::vec4 Material_PBR::getTexMultiplier(MaterialTexType_PBR type) const {
    glm::vec4 result;
    data->get(result, MaterialGLdata_PBR::OffsetOfTexMultiplier(type));
    return result;
}

void Material_PBR::changeTexMultiplier(MaterialTexType_PBR type,
                                       const Changer<glm::vec4>& changer) {
    data->change(changer, MaterialGLdata_PBR::OffsetOfTexMultiplier(type));
}

} // namespace gnev