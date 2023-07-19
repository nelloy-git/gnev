#pragma once

#include "glad/gl.h"

namespace gnev {

enum class VertAttribType : GLenum {
    Float = GL_FLOAT,
    Int = GL_INT,
    UInt = GL_UNSIGNED_INT,
    Byte = GL_BYTE,
    UByte = GL_UNSIGNED_BYTE,
    Short = GL_SHORT,
    UShort = GL_UNSIGNED_SHORT,

    UInt_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
};

template<VertAttribType T, GLuint S, bool N>
struct VertAttrib final {
    static constexpr GLuint Elements = S;
    static constexpr GLenum GLType = static_cast<GLenum>(T);
    static constexpr bool Norm = N;
    static constexpr bool IsPacked = is_packed_type(T);

    static_assert(Elements <= 4, "Vertex attribute can have maximum 4 components");
    static_assert(Elements >  0, "Vertex attribute can have minimum 1 component");

    static void write(void* dst, const GLfloat* src);
    static void write(void* dst, const GLint* src);
    static void write(void* dst, const GLuint* src);

private:
    static constexpr bool is_packed_type();
};

template<VertAttribType T, GLuint S, bool N>
constexpr bool VertAttrib<T, S, N>::is_packed_type(){
    using enum VertAttribType;
    if constexpr (T == UInt_10_10_10_2){
        return true;
    } else {
        return false;
    }
}

}