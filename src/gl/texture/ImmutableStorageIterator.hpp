#pragma once

#include <variant>

#include "gl/Texture.hpp"
#include "gl/texture/Image.hpp"

namespace gnev::gl::texture {

class ImmutableStorage;

class EXPORT ImmutableStorageIterator {
public:
    ImmutableStorageIterator(ImmutableStorage& storage, GLuint index);
    ImmutableStorageIterator(const ImmutableStorage& storage, GLuint index);
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
    GLuint getBufferSize(const ImageInfo& info) const;

protected:
    ImmutableStorage& getStorage();
    const ImmutableStorage& getStorage() const;

private:
    template <typename T>
    using reference = std::reference_wrapper<T>;

    std::variant<reference<ImmutableStorage>, reference<const ImmutableStorage>> storage;
    GLuint index;

    static GLuint getComponents(GLenum format);
    static double getBytesPerElement(GLenum type);
};

} // namespace gnev::gl::texture