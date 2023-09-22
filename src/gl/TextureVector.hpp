#pragma once

#include "gl/Texture.hpp"

namespace gnev::gl {

class EXPORT TextureVector : public Texture {
public:
    static constexpr GLsizeiptr base_cap = 4;

    struct Image {
        GLint level;
        GLsizei x;
        GLsizei y;
        GLsizei width;
        GLsizei height;
        GLenum format;
        GLenum type;
        std::shared_ptr<void> data;
    };

    TextureVector(const std::shared_ptr<GladGLContext>& ctx,
                  GLsizei levels,
                  GLenum internalformat,
                  GLsizei width,
                  GLsizei height);
    virtual ~TextureVector();

    // std::unique_ptr<Image, void(*)(Image*)> get(GLsizeiptr i, GLint level) const; TODO
    void set(GLsizeiptr i, const Image& value);

    GLsizeiptr size() const;
    void reserve(GLsizeiptr capacity);
    GLsizeiptr capacity() const;
    void shrink_to_fit();
    GLsizeiptr max_size() const;

    // void clear();
    void insert(GLsizeiptr i, const Image& value);
    void insert_range(GLsizeiptr i, const Image* value, GLuint count);
    // void erase(GLsizeiptr i);
    // void erase_range(GLsizeiptr i, GLuint count);
    void push_back(const Image& value);
    void push_back_range(const Image* values, GLuint count);
    // void pop_back();
    // void pop_back_range(GLuint count);

    void set_resize_params(float mult, float add);

private:
    GLsizei _img_levels;
    GLenum _img_internalformat;

    GLsizeiptr _size;
    GLsizeiptr _cap;
    GLsizeiptr _max_size;

    double _cap_mult = 2;
    double _cap_add = 0;

    GLint _get_width(GLint level) const;
    GLint _get_height(GLint level) const;

    static void* _alloc(size_t size);
    static void _free(void* ptr);
};

} // namespace gnev::gl