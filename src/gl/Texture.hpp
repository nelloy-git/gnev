#pragma once

#include "gl/Handler.hpp"
#include "gl/enum/TextureFormat.hpp"
#include "gl/enum/TextureInternalFormat.hpp"
#include "gl/enum/TextureMagFilter.hpp"
#include "gl/enum/TextureMinFilter.hpp"
#include "gl/enum/TextureTarget.hpp"
#include "gl/enum/TextureType.hpp"
#include "gl/enum/TextureWrapS.hpp"
#include "gl/enum/TextureWrapT.hpp"

namespace gnev::gl {

class EXPORT Texture : public Handler {
public:
    Texture(GLenum target);
    Texture(const Texture&) = delete;
    Texture(Texture&&) = default;
    virtual ~Texture();

    void bind(TextureTarget target) const;

    TextureTarget getTarget() const;
    GLuint getWidth(GLuint level) const;
    GLuint getHeight(GLuint level) const;
    GLuint getDepth(GLuint level) const;

    void setWrapS(TextureWrapS wrap);
    void setWrapT(TextureWrapT wrap);
    void setMinFilter(TextureMinFilter filter);
    void setMagFilter(TextureMagFilter filter);

    void generateMipmap();
    void initStorage3D(GLuint levels,
                       TextureInternalFormat internal_format,
                       GLuint width,
                       GLuint height,
                       GLuint depth);
    void setSubImage3D(GLuint level,
                       GLuint x,
                       GLuint y,
                       GLuint z,
                       GLuint width,
                       GLuint height,
                       GLuint depth,
                       TextureFormat format,
                       TextureType type,
                       const void* pixels);
    void getSubImage(GLuint level,
                     GLuint x,
                     GLuint y,
                     GLuint z,
                     GLuint width,
                     GLuint height,
                     GLuint depth,
                     TextureFormat format,
                     TextureType type,
                     GLuint bufSize,
                     void* pixels) const;
    void copyTo(Texture& dst,
                GLuint src_level,
                GLuint src_x,
                GLuint src_y,
                GLuint src_z,
                GLuint dstlevel,
                GLuint dstx,
                GLuint dsty,
                GLuint dstz,
                GLuint srcWidth,
                GLuint srcHeight,
                GLuint srcDepth) const;

private:
    static GLuint* createHandle(GLenum target);
    static void deleteHandle(GLuint* handle);
};

} // namespace gnev::gl