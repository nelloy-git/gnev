#pragma once

#include <array>

#include "glad/gl.h"

namespace gnev {

struct Attribute {
    constexpr Attribute(GLuint elements, GLenum type, bool normalized)
        : elements(elements)
    {};

    const GLuint elements;

};

enum class VertAttribGLType : GLenum {
    Float = GL_FLOAT,
    Int = GL_INT,
    UInt = GL_UNSIGNED_INT,
    Byte = GL_BYTE,
    UByte = GL_UNSIGNED_BYTE,
    Short = GL_SHORT,
    UShort = GL_UNSIGNED_SHORT,

    UInt_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
};

template<VertAttribGLType>
struct VertAttribViewType {};

template<VertAttribGLType T, GLuint S, bool N>
struct VertAttrib final
{
private:
    static constexpr bool is_packed_type();
    static constexpr bool is_signed_type();

public:

    using ViewType = VertAttribViewType<T>::T;

    static constexpr GLuint Elements = S;
    static constexpr GLenum GLType = static_cast<GLenum>(T);
    static constexpr bool Norm = N;
    static constexpr bool IsPacked = is_packed_type();
    static constexpr bool IsSigned = std::is_signed_v<ViewType>;

    static_assert(Elements <= 4, "Vertex attribute can have maximum 4 components");
    static_assert(Elements >  0, "Vertex attribute can have minimum 1 component");
    static_assert(!(T == VertAttribGLType::UInt_10_10_10_2 && Elements != 4), "UInt_10_10_10_2 can be 4 components only");
    static_assert(sizeof(std::array<ViewType, Elements>) == sizeof(ViewType[Elements]), "Invalid ViewType");
 
    static void write(void* dst, const GLfloat* src);
    static void write(void* dst, const GLint* src);
    static void write(void* dst, const GLuint* src);

    void write(const GLfloat* src);
    void write(const GLint* src);
    void write(const GLuint* src);

    std::array<ViewType, IsPacked ? 1 : Elements> raw;

private:

};
        
template<template<VertAttribGLType, GLuint, bool> class base, typename derived>
struct is_VertAttrib_impl
{
    template<VertAttribGLType T, GLuint S, bool N>
    static constexpr std::true_type  test(const base<T, S, N> *);
    static constexpr std::false_type test(...);
    static constexpr bool value = decltype(test(std::declval<derived*>()))::value;
};

template<typename T>
concept VertAttribT = is_VertAttrib_impl<VertAttrib, T>::value;

template<VertAttribGLType T, GLuint S, bool N>
void VertAttrib<T, S, N>::write(void* dst, const GLfloat* src)
{
    constexpr Attribute a(45);
    if constexpr (IsPacked){
        []<bool B = false>(){static_assert(B, "Unsupported ViewType");}();
    } else {
        for (int i = 0; i < Elements; ++i){
            if constexpr (!Norm){
                raw[i] = src[i];
            } else {
                GLdouble normed;
                if constexpr (IsSigned){
                    normed = std::max(-1, std::min(1, src[i]));
                } else {
                    normed = std::max(0, std::min(1, src[i]));
                }

                if constexpr (GLType == GL_FLOAT){
                    raw[i] = normed;
                } else {
                    raw[i] = normed >= 0 ? normed * std::numeric_limits<ViewType>::max() : normed * std::numeric_limits<ViewType>::min();
                }
            }
        }
    }
}

template<VertAttribGLType T, GLuint S, bool N>
void VertAttrib<T, S, N>::write(void* dst, const GLint* src)
{
    if constexpr (IsPacked){
        []<bool B = false>(){static_assert(B, "Unsupported ViewType");}();
    } else {
        for (int i = 0; i < Elements; ++i){
            if constexpr (!Norm){
                raw[i] = src[i];
            } else {
                if constexpr (GLType == GL_INT){
                    raw[i] = src[i];
                } else {
                    GLdouble normed = src[i] >= 0 ? src[i] / std::numeric_limits<GLint>::max() : src[i] / std::numeric_limits<GLint>::min();
                    if constexpr (GLType == GL_FLOAT){
                        raw[i] = normed;
                    } else {
                        raw[i] = normed >= 0 ? normed * std::numeric_limits<ViewType>::max() : normed * std::numeric_limits<ViewType>::min();
                    }
                }
            }
        }
    }
}

template<VertAttribGLType T, GLuint S, bool N>
void VertAttrib<T, S, N>::write(void* dst, const GLuint* src)
{
    if constexpr (IsPacked){
        []<bool B = false>(){static_assert(B, "Unsupported ViewType");}();
    } else {
        for (int i = 0; i < Elements; ++i){
            if constexpr (!Norm){
                raw[i] = src[i];
            } else {
                if constexpr (GLType == GL_UNSIGNED_INT){
                    raw[i] = src[i];
                } else {
                    GLdouble normed = src[i] / std::numeric_limits<GLuint>::max();
                    if constexpr (GLType == GL_FLOAT){
                        raw[i] = normed;
                    } else {
                        raw[i] = normed * std::numeric_limits<ViewType>::max();
                    }
                }
            }
        }
    }
}

template<VertAttribGLType T, GLuint S, bool N>
void VertAttrib<T, S, N>::write(const GLfloat* src){
    write(&raw, src);
}

template<VertAttribGLType T, GLuint S, bool N>
void VertAttrib<T, S, N>::write(const GLint* src){
    write(&raw, src);
}

template<VertAttribGLType T, GLuint S, bool N>
void VertAttrib<T, S, N>::write(const GLuint* src){
    write(&raw, src);
}

template<VertAttribGLType T, GLuint S, bool N>
constexpr bool VertAttrib<T, S, N>::is_packed_type()
{
    using enum VertAttribGLType;
    if constexpr (T == UInt_10_10_10_2){
        return true;
    } else {
        return false;
    }
}

template<VertAttribGLType T, GLuint S, bool N>
constexpr bool VertAttrib<T, S, N>::is_signed_type()
{
    using enum VertAttribGLType;
    if constexpr (T == GL_FLOAT){
        return true;
    } else {
        return false;
    }
}

template<>
struct VertAttribViewType<VertAttribGLType::Float>
{
    using T = GLfloat;
};

template<>
struct VertAttribViewType<VertAttribGLType::Int>
{
    using T = GLint;
};

template<>
struct VertAttribViewType<VertAttribGLType::UInt>
{
    using T = GLuint;
};

template<>
struct VertAttribViewType<VertAttribGLType::Byte>
{
    using T = GLbyte;
};

template<>
struct VertAttribViewType<VertAttribGLType::UByte>
{
    using T = GLubyte;
};

template<>
struct VertAttribViewType<VertAttribGLType::Short>
{
    using T = GLshort;
};

template<>
struct VertAttribViewType<VertAttribGLType::UShort>
{
    using T = GLushort;
};

template<>
struct VertAttribViewType<VertAttribGLType::UInt_10_10_10_2>
{
    using T = GLuint;
};


}