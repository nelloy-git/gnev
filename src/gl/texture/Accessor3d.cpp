#include "gl/texture/Accessor3d.hpp"

namespace gnev::gl::texture {

Accessor3d::Accessor3d(Ref<Texture> texture)
    : texture(texture) {
    auto target = texture->getTarget();
    if (target != TextureTarget::TEXTURE_3D and
        target != TextureTarget::TEXTURE_2D_ARRAY) {
        throw std::logic_error("");
    }
}

Accessor3dSubImage::Accessor3dSubImage(Ref<Texture> texture)
    : Accessor3d(texture) {}

void Accessor3dSubImage::set(const Image3d& image) {
    auto& info = image.info;
    texture->setSubImage3D(info.level,
                           info.x,
                           info.y,
                           info.z,
                           info.width,
                           info.height,
                           info.depth,
                           info.format,
                           info.type,
                           image.data.get());
}

void Accessor3dSubImage::get(Image3d& image) const {
    auto& info = image.info;
    texture->getSubImage(info.level,
                         info.x,
                         info.y,
                         info.z,
                         info.width,
                         info.height,
                         info.depth,
                         info.format,
                         info.type,
                         image.data.buffer_size,
                         image.data.get());
}

void Accessor3dSubImage::change(const ImageInfo3d& info, Changer changer) {
    Image3d img{info, ImageData{info.calcTextureBufferSize()}};
    get(img);
    changer(img);
    set(img);
}

} // namespace gnev::gl::texture