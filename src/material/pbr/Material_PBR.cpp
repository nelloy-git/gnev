#include "material/pbr/Material_PBR.hpp"

#include <cstddef>
#include <memory>
#include <stdexcept>

#include "material/base/Material.hpp"
#include "material/pbr/MaterialGL_PBR.hpp"

namespace gnev {

namespace {

inline unsigned int toUint(MaterialTexRefType_PBR t) {
    return static_cast<unsigned int>(t);
}

} // namespace

Material_PBR::Material_PBR(MaterialStorage_PBR& storage)
    : base::Material<MaterialGL_PBR>(storage) {}

Material_PBR::~Material_PBR() {}

void Material_PBR::setTextureIndex(MaterialTexRefType_PBR type, GLuint value) {
    auto store = getDataRef().lockStorage();
    auto iter = store->at(*getDataRef().index);
    iter->tex_index[toUint(type)] = value;
}

GLuint Material_PBR::getTextureIndex(MaterialTexRefType_PBR type) const {
    std::size_t offset =
        offsetof(MaterialDataPBR, tex_index) + toUint(type) * sizeof(GLuint);
    GLuint value;
    getDataIterator().copyFrom(&value, offset, sizeof(value));
    return value;
}

void Material_PBR::uploadTexture(MaterialTexRefType_PBR type,
                                 const std::filesystem::path& path,
                                 GLuint level) {
    auto storage = getStorage();
    auto tex_storage = storage->getTexStorage(toUint(type));
    auto loader = storage->getImageLoader(toUint(type));

    auto index_opt = tex_storage->initUnusedIndex();
    if (not index_opt.has_value()) {
        throw std::out_of_range("");
    }

    gl::TexImageInfo info{.level = level,
                          .x = 0,
                          .y = 0,
                          .width = tex_storage->getLevelWidth(level),
                          .height = tex_storage->getLevelHeight(level),
                          .format = GL_RGBA8,
                          .type = GL_UNSIGNED_BYTE};

    loader->upload(tex_storage, index_opt.value(), path, info);
}

void Material_PBR::setTextureOffset(MaterialTexRefType_PBR type,
                                    const glm::vec4& value) {
    std::size_t offset =
        offsetof(MaterialDataPBR, tex_offset) + toUint(type) * sizeof(glm::vec4);
    getDataIterator().copyTo(&value, offset, sizeof(value));
}

glm::vec4 Material_PBR::getTextureOffset(MaterialTexRefType_PBR type) const {
    std::size_t offset =
        offsetof(MaterialDataPBR, tex_offset) + toUint(type) * sizeof(glm::vec4);
    glm::vec4 value;
    getDataIterator().copyFrom(&value, offset, sizeof(value));
    return value;
}

void Material_PBR::setTextureMultiplier(MaterialTexRefType_PBR type,
                                        const glm::vec4& value) {
    std::size_t offset =
        offsetof(MaterialDataPBR, tex_multiplier) + toUint(type) * sizeof(glm::vec4);
    getDataIterator().copyTo(&value, offset, sizeof(value));
}

glm::vec4 Material_PBR::getTextureMultiplier(MaterialTexRefType_PBR type) const {
    std::size_t offset =
        offsetof(MaterialDataPBR, tex_multiplier) + toUint(type) * sizeof(glm::vec4);
    glm::vec4 value;
    getDataIterator().copyFrom(&value, offset, sizeof(value));
    return value;
}

} // namespace gnev