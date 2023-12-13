#pragma once

#include "gl/Texture.hpp"
#include "image/Image.hpp"
#include "util/Ref.hpp"

namespace gnev::gl::texture {

class Accessor3d {
public:
    using Changer = std::function<void(Image3d&)>;

    Accessor3d(Ref<gl::Texture>);
    virtual ~Accessor3d() = default;

    Ref<gl::Texture> texture;

    virtual void set(const Image3d& image) = 0;
    virtual void get(Image3d& image) const = 0;
    virtual void change(const ImageInfo3d& info, Changer changer) = 0;

    GLenum getTarget() const;
    GLuint getWidth(GLuint level) const;
    GLuint getHeight(GLuint level) const;
    GLuint getDepth() const;
};

class Accessor3dSubImage : public Accessor3d {
public:
    Accessor3dSubImage(Ref<gl::Texture>);
    virtual ~Accessor3dSubImage() = default;

    virtual void set(const Image3d& image) override;
    virtual void get(Image3d& image) const override;
    virtual void change(const ImageInfo3d& info, Changer changer) override;
};

} // namespace gnev::gl::texture
