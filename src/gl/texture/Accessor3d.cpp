#include "gl/texture/Accessor3d.hpp"

namespace gnev::gl::texture {

Accessor3d::Accessor3d(Ref<Texture> texture)
    : texture(texture) {
    GLenum target = getTarget();
    if (target != GL_TEXTURE_3D and target != GL_TEXTURE_2D_ARRAY) {
        throw std::logic_error("");
    }
}

GLuint Accessor3d::getTarget() const {
    int target;
    texture->getParameteriv(GL_TEXTURE_TARGET, &target);
    return target;
}

GLuint Accessor3d::getWidth(GLuint level) const {
    int width;
    texture->getLevelParameteriv(level, GL_TEXTURE_WIDTH, &width);
    return width;
}

GLuint Accessor3d::getHeight(GLuint level) const {
    int height;
    texture->getLevelParameteriv(level, GL_TEXTURE_HEIGHT, &height);
    return height;
}

GLuint Accessor3d::getDepth() const {
    int depth;
    texture->getLevelParameteriv(0, GL_TEXTURE_DEPTH, &depth);
    return depth;
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