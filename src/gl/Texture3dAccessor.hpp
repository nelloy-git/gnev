#pragma once

#include "gl/Texture.hpp"
#include "image/Image.hpp"
#include "util/Ref.hpp"

namespace gnev::gl {

class Texture3DAccessor {
public:
    using Changer = std::function<void(Texture3DAccessor&, GLuint z, const Image&)>;

    Texture3DAccessor(Ref<gl::Texture>);
    virtual ~Texture3DAccessor() = default;

    Ref<gl::Texture> texture;

    virtual void set(GLuint z, const Image& image) = 0;
    virtual void get(GLuint z, Image& image) = 0;
    virtual void change(GLuint z, const ImageInfo& info, Changer changer) = 0;
};

class Texture3DAccessorSubImage : public Texture3DAccessor {
public:
    Texture3DAccessorSubImage(Ref<gl::Texture>);
    virtual ~Texture3DAccessorSubImage() = default;

    virtual void set(GLuint z, const Image& image) override;
    virtual void get(GLuint z, Image& image) override;
    virtual void change(GLuint z, const ImageInfo& info, Changer changer) override;
};

} // namespace gnev::gl
