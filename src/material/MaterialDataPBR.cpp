#include "material/MaterialDataPBR.hpp"

namespace gnev {

namespace {

inline unsigned int toUint(ParamPBR p) { return static_cast<unsigned int>(p); }

} // namespace

MaterialDataPBR::Index MaterialDataPBR::getIndex() { return self_index; }

void MaterialDataPBR::setOffset(ParamPBR param, const glm::vec4& value) {
    tex_offset[toUint(param)] = value;
}

const glm::vec4& MaterialDataPBR::getOffset(ParamPBR param) {
    return tex_offset[toUint(param)];
}

void MaterialDataPBR::setScale(ParamPBR param, const glm::vec4& value) {
    tex_scale[toUint(param)] = value;
}

const glm::vec4& MaterialDataPBR::getScale(ParamPBR param) {
    return tex_scale[toUint(param)];
}

void MaterialDataPBR::setTexIndex(ParamPBR param, std::size_t value) {
    tex_index[toUint(param)] = value;
}

std::size_t MaterialDataPBR::getTexIndex(ParamPBR param) {
    return tex_index[toUint(param)];
}

} // namespace gnev