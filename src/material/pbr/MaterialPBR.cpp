#include "material/pbr/MaterialPBR.hpp"

#include <cstddef>
#include <memory>
#include <stdexcept>

#include "material/base/Material.hpp"
#include "material/base/MaterialStorage.hpp"
#include "material/pbr/MaterialDataPBR.hpp"

namespace gnev {

namespace {

inline unsigned int toUint(ParamPBR p) { return static_cast<unsigned int>(p); }

} // namespace

MaterialPBR::MaterialPBR(const std::shared_ptr<Storage>& storage, GLuint data_index)
    : Base(storage, data_index) {}

MaterialPBR::~MaterialPBR() {}

void MaterialPBR::setTextureIndex(ParamPBR param, GLuint value) {
    std::size_t offset =
        offsetof(MaterialDataPBR, tex_index) + toUint(param) * sizeof(GLuint);
    getDataIterator().copyTo(&value, offset, sizeof(value));
}

GLuint MaterialPBR::getTextureIndex(ParamPBR param) const {
    std::size_t offset =
        offsetof(MaterialDataPBR, tex_index) + toUint(param) * sizeof(GLuint);
    GLuint value;
    getDataIterator().copyFrom(&value, offset, sizeof(value));
    return value;
}

void MaterialPBR::setTextureOffset(ParamPBR param, const glm::vec4& value) {
    std::size_t offset =
        offsetof(MaterialDataPBR, tex_offset) + toUint(param) * sizeof(glm::vec4);
    getDataIterator().copyTo(&value, offset, sizeof(value));
}

glm::vec4 MaterialPBR::getTextureOffset(ParamPBR param) const {
    std::size_t offset =
        offsetof(MaterialDataPBR, tex_offset) + toUint(param) * sizeof(glm::vec4);
    glm::vec4 value;
    getDataIterator().copyFrom(&value, offset, sizeof(value));
    return value;
}

void MaterialPBR::setTextureMultiplier(ParamPBR param, const glm::vec4& value) {
    std::size_t offset =
        offsetof(MaterialDataPBR, tex_multiplier) + toUint(param) * sizeof(glm::vec4);
    getDataIterator().copyTo(&value, offset, sizeof(value));
}

glm::vec4 MaterialPBR::getTextureMultiplier(ParamPBR param) const {
    std::size_t offset =
        offsetof(MaterialDataPBR, tex_multiplier) + toUint(param) * sizeof(glm::vec4);
    glm::vec4 value;
    getDataIterator().copyFrom(&value, offset, sizeof(value));
    return value;
}

} // namespace gnev