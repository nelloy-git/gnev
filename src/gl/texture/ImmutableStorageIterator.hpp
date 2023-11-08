#pragma once

#include <variant>

#include "gl/Texture.hpp"
#include "gl/texture/Image.hpp"

namespace gnev::gl::texture {

class EXPORT ImmutableStorageIterator {
    friend class ImmutableStorage;

public:
    virtual ~ImmutableStorageIterator();

    GLuint getIndex() const;

    Image getImage(const ImageInfo& info) const;
    Image getImage(const ImageInfo& info, const ImageData& dst) const;
    Image getImage(GLuint level, GLenum format, GLenum type) const;
    void getImage(Image& dst) const;
    void setImage(const Image& img);

    GLuint getLevels() const;
    GLenum getLevelInternalFormat(GLuint level) const;
    GLuint getLevelWidth(GLuint level) const;
    GLuint getLevelHeight(GLuint level) const;
    GLuint getImageBufferSize(const ImageInfo& info) const;

protected:
    ImmutableStorageIterator(ImmutableStorage& storage, GLuint index);
    ImmutableStorageIterator(const ImmutableStorage& storage, GLuint index);

    ImmutableStorage& getStorage();
    const ImmutableStorage& getStorage() const;

private:
    template <typename T>
    using reference = std::reference_wrapper<T>;

    std::variant<reference<ImmutableStorage>, reference<const ImmutableStorage>> storage;
    GLuint index;
};

} // namespace gnev::gl::texture