#include "gl/Texture3dAccessor.hpp"

namespace gnev::gl {

Texture3DAccessor::Texture3DAccessor(Ref<Texture> texture)
    : texture(texture) {
    int target;
    texture->getParameteriv(GL_TEXTURE_TARGET, &target);
    if (target != GL_TEXTURE_3D and target != GL_TEXTURE_2D_ARRAY) {
        throw std::logic_error("");
    }
}

Texture3DAccessorSubImage::Texture3DAccessorSubImage(Ref<Texture> texture)
    : Texture3DAccessor(texture) {}

void Texture3DAccessorSubImage::set(GLuint z, const Image& image) {
    auto& info = image.info;
    texture->setSubImage3D(info.level,
                           info.x,
                           info.y,
                           z,
                           info.width,
                           info.height,
                           1,
                           info.format,
                           info.type,
                           image.data.get());
}

void Texture3DAccessorSubImage::get(GLuint z, Image& image) {
    auto& info = image.info;
    texture->getSubImage(info.level,
                         info.x,
                         info.y,
                         z,
                         info.width,
                         info.height,
                         1,
                         info.format,
                         info.type,
                         image.data.buffer_size,
                         image.data.get());
}

void Texture3DAccessorSubImage::change(GLuint z, const ImageInfo& info, Changer changer) {
    Image img{info, ImageData{info.getTextureBufferSize()}};
    get(z, img);
    changer(*this, z, img);
    set(z, img);
}

} // namespace gnev::gl