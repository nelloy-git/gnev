#pragma once

#include <unordered_set>

#include "gl/Texture.hpp"
#include "material/base/MaterialImage.hpp"
#include "util/Export.hpp"
#include "util/Ref.hpp"

namespace gnev::base {

class EXPORT MaterialTexStorage {
public:
    // clang-format off
    using Setter =
        std::function<bool(gl::Texture& texture, GLuint index, const gl::TexImage& src)>;
    using Getter =
        std::function<bool(const gl::Texture& texture, GLuint index, gl::TexImage& dst)>;
    // clang-format on

    MaterialTexStorage(Ref<gl::Texture> texture, GLuint capacity);
    MaterialTexStorage(GLuint levels,
                       GLenum internal_format,
                       GLuint width,
                       GLuint height,
                       GLuint capacity);
    virtual ~MaterialTexStorage();

    Ref<gl::Texture> getTexture() const;
    void setSetter(const Setter& setter);
    void setGetter(const Getter& getter);

    GLuint initIndex();
    void freeIndex(GLuint index);
    bool hasIndex(GLuint index) const;
    GLuint unusedCount() const;

    bool setData(GLuint index, const gl::TexImage& src);
    bool getData(GLuint index, gl::TexImage& src) const;

    // Default setter
    static bool setSubImage(gl::Texture& texture, GLuint index, const gl::TexImage& src);
    // Default getter
    static bool getSubImage(const gl::Texture& texture, GLuint index, gl::TexImage& dst);

private:
    Ref<gl::Texture> texture;
    Setter texture_setter;
    Getter texture_getter;
    GLuint capacity;

    std::unordered_set<GLuint> unused;

    static Ref<gl::Texture> initTexture(GLuint levels,
                                        GLuint width,
                                        GLuint height,
                                        GLuint capacity,
                                        GLenum internal_format);
};

} // namespace gnev::base