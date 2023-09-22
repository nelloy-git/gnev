#pragma once

#include <type_traits>

#include "glad/gl.h"

namespace gnev::data {

namespace details {

template <typename T>
constexpr bool is_valid_index_type =
    std::is_same_v<T, GLubyte> || std::is_same_v<T, GLushort> || std::is_same_v<T, GLuint>;

template <typename T>
constexpr GLenum get_index_enum() {
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
concept IsIndex = details::is_valid_index_type<T>;

template <IsIndex T>
constexpr GLenum IndexEnum = details::get_index_enum<T>();

} // namespace gnev::data