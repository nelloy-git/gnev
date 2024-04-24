#pragma once

#include "Ctx.hpp"
#include "magic_enum/magic_enum.hpp"

namespace gnev::gl {

enum class BufferStorageFlags : GLenum {
    EMPTY = 0,
    DYNAMIC_STORAGE_BIT = GL_DYNAMIC_STORAGE_BIT,
    MAP_READ_BIT = GL_MAP_READ_BIT,
    MAP_WRITE_BIT = GL_MAP_WRITE_BIT,
    MAP_PERSISTENT_BIT = GL_MAP_PERSISTENT_BIT,
    MAP_COHERENT_BIT = GL_MAP_COHERENT_BIT,
    CLIENT_STORAGE_BIT = GL_CLIENT_STORAGE_BIT,
};

inline constexpr BufferStorageFlags operator~(BufferStorageFlags rhs) {
    return magic_enum::bitwise_operators::operator~(rhs);
}

inline constexpr BufferStorageFlags operator|(BufferStorageFlags lhs,
                                              BufferStorageFlags rhs) {
    return magic_enum::bitwise_operators::operator|(lhs, rhs);
}

inline constexpr BufferStorageFlags operator&(BufferStorageFlags lhs,
                                              BufferStorageFlags rhs) {
    return magic_enum::bitwise_operators::operator&(lhs, rhs);
}

inline constexpr BufferStorageFlags operator^(BufferStorageFlags lhs,
                                              BufferStorageFlags rhs) {
    return magic_enum::bitwise_operators::operator^(lhs, rhs);
}

inline constexpr BufferStorageFlags operator|=(BufferStorageFlags lhs,
                                               BufferStorageFlags rhs) {
    return magic_enum::bitwise_operators::operator|=(lhs, rhs);
}

inline constexpr BufferStorageFlags operator&=(BufferStorageFlags lhs,
                                               BufferStorageFlags rhs) {
    return magic_enum::bitwise_operators::operator&=(lhs, rhs);
}

inline constexpr BufferStorageFlags operator^=(BufferStorageFlags lhs,
                                               BufferStorageFlags rhs) {
    return magic_enum::bitwise_operators::operator^=(lhs, rhs);
}

std::string_view format_as(BufferStorageFlags value);

} // namespace gnev::gl

namespace magic_enum::customize {

template <>
struct enum_range<gnev::gl::BufferStorageFlags> {
    static constexpr bool is_flags = true;
};

} // namespace magic_enum::customize
