#include "material/base/MaterialTexStorage.hpp"

namespace gnev::base {

using Setter =
    std::function<bool(gl::Texture& buffer, GLuint offset, GLuint size, const void* src)>;
using Getter =
    std::function<bool(const gl::Texture& buffer, GLuint offset, GLuint size, void* dst)>;

MaterialTexStorage::MaterialTexStorage(Ref<gl::Texture> texture, GLuint capacity)
    : texture(texture)
    , texture_setter(&setSubImage)
    , texture_getter(&getSubImage)
    , capacity(capacity) {
    for (GLuint i = 0; i < capacity; ++i) {
        unused.emplace(capacity - i - 1);
    }
}

MaterialTexStorage::MaterialTexStorage(GLuint levels,
                                       GLenum internal_format,
                                       GLuint width,
                                       GLuint height,
                                       GLuint capacity)
    : MaterialTexStorage(initTexture(levels, internal_format, width, height, capacity),
                         capacity) {}

MaterialTexStorage::~MaterialTexStorage() {}

Ref<gl::Texture> MaterialTexStorage::getTexture() const { return texture; }

void MaterialTexStorage::setSetter(const Setter& setter) { texture_setter = setter; }

void MaterialTexStorage::setGetter(const Getter& getter) { texture_getter = getter; }

GLuint MaterialTexStorage::initIndex() {
    auto iter = unused.begin();
    if (iter == unused.end()) {
        throw std::out_of_range("");
    }
    GLuint index = *iter;
    unused.erase(iter);
    return index;
}

void MaterialTexStorage::freeIndex(GLuint index) { unused.insert(unused.end(), index); }

bool MaterialTexStorage::hasIndex(GLuint index) const {
    return not unused.contains(index);
}

GLuint MaterialTexStorage::unusedCount() const { return unused.size(); }

bool MaterialTexStorage::setData(GLuint index, const gl::TexImage& src) {
    return texture_setter(texture, index, src);
}

bool MaterialTexStorage::getData(GLuint index, gl::TexImage& src) const {
    return texture_getter(texture, index, src);
}

bool MaterialTexStorage::setSubImage(gl::Texture& texture,
                                     GLuint index,
                                     const gl::TexImage& src) {
    auto& info = src.info;
    texture.setSubImage3D(info.level,
                          info.x,
                          info.y,
                          index,
                          info.width,
                          info.height,
                          1,
                          info.format,
                          info.type,
                          src.data.data());
    return true;
}

bool MaterialTexStorage::getSubImage(const gl::Texture& texture,
                                     GLuint index,
                                     gl::TexImage& dst) {
    const auto& info = dst.info;
    texture.getSubImage(info.level,
                        info.x,
                        info.y,
                        index,
                        info.width,
                        info.height,
                        1,
                        info.format,
                        info.type,
                        dst.data.size(),
                        dst.data.data());
    return true;
}

Ref<gl::Texture> MaterialTexStorage::initTexture(GLuint levels,
                                                 GLenum internal_format,
                                                 GLuint width,
                                                 GLuint height,
                                                 GLuint capacity) {
    auto texture = Ref<gl::Texture>::Make(GL_TEXTURE_2D_ARRAY);
    texture->initStorage3D(levels, internal_format, width, height, capacity);
    return texture;
}

} // namespace gnev::base