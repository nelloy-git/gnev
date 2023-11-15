#pragma once

#include <variant>

#include "gl/Texture.hpp"
#include "gl/texture/TexImage.hpp"

namespace gnev::gl {

class TexStorage;

class EXPORT TexStorageIterator {
    friend TexStorage;

public:
    virtual ~TexStorageIterator();

    GLuint getIndex() const;

    TexImage getImage(const TexImageInfo& info) const;
    TexImage getImage(const TexImageInfo& info, const TexImageData& dst) const;
    TexImage getImage(GLuint level, GLenum format, GLenum type) const;
    void getImage(TexImage& dst) const;
    void setImage(const TexImage& img);

    GLuint getLevels() const;
    GLenum getLevelInternalFormat(GLuint level) const;
    GLuint getLevelWidth(GLuint level) const;
    GLuint getLevelHeight(GLuint level) const;
    GLuint getImageBufferSize(const TexImageInfo& info) const;

protected:
    TexStorageIterator(TexStorage& storage, GLuint index);
    TexStorageIterator(const TexStorage& storage, GLuint index);

    TexStorage& getStorage();
    const TexStorage& getStorage() const;

private:
    template <typename V>
    using reference = std::reference_wrapper<V>;

    std::variant<reference<TexStorage>, reference<const TexStorage>> storage;
    GLuint index;
};

} // namespace gnev::gl