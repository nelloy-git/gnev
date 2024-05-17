#pragma once

#include <stdexcept>
#include <type_traits>

#include "glad/gl.h"

namespace gnev::gl {

namespace details {

template <typename T>
constexpr bool IsValidIndexType =
    std::is_same_v<T, GLubyte> || std::is_same_v<T, GLushort> || std::is_same_v<T, GLuint>;

template <typename T>
consteval GLenum GetIndexEnum() {
    if constexpr (std::is_same_v<T, GLubyte>) {
        return GL_UNSIGNED_BYTE;
    } else if constexpr (std::is_same_v<T, GLushort>) {
        return GL_UNSIGNED_SHORT;
    } else if constexpr (std::is_same_v<T, GLuint>) {
        return GL_UNSIGNED_INT;
    } else {
        []<bool B = true>() { static_assert(B, ""); }
        ();
    }
}

}; // namespace details

template <typename T>
concept IsIndex = details::IsValidIndexType<T>;

template <IsIndex T>
constexpr GLenum IndexEnum = details::GetIndexEnum<T>();

constexpr std::size_t GetIndexSize(GLenum index_enum) {
    switch (index_enum){
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        case GL_UNSIGNED_SHORT: return sizeof(GLushort);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        default: throw std::out_of_range("");
    }
}

} // namespace gnev::gl::data