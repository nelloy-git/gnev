#pragma once

#include "Ctx.hpp"
#include "magic_enum/magic_enum.hpp"

namespace gnev::gl {

enum class BufferMapRangeAccess : GLenum {
    MAP_READ_BIT = GL_MAP_READ_BIT,
    MAP_WRITE_BIT = GL_MAP_WRITE_BIT,
    MAP_PERSISTENT_BIT = GL_MAP_PERSISTENT_BIT,
    MAP_COHERENT_BIT = GL_MAP_COHERENT_BIT,
    MAP_INVALIDATE_RANGE_BIT = GL_MAP_INVALIDATE_RANGE_BIT,
    MAP_INVALIDATE_BUFFER_BIT = GL_MAP_INVALIDATE_BUFFER_BIT,
    MAP_FLUSH_EXPLICIT_BIT = GL_MAP_FLUSH_EXPLICIT_BIT,
    MAP_UNSYNCHRONIZED_BIT = GL_MAP_UNSYNCHRONIZED_BIT
};

inline constexpr BufferMapRangeAccess operator~(BufferMapRangeAccess rhs) {
    return magic_enum::bitwise_operators::operator~(rhs);
}

inline constexpr BufferMapRangeAccess operator|(BufferMapRangeAccess lhs,
                                                BufferMapRangeAccess rhs) {
    return magic_enum::bitwise_operators::operator|(lhs, rhs);
}

inline constexpr BufferMapRangeAccess operator&(BufferMapRangeAccess lhs,
                                                BufferMapRangeAccess rhs) {
    return magic_enum::bitwise_operators::operator&(lhs, rhs);
}

inline constexpr BufferMapRangeAccess operator^(BufferMapRangeAccess lhs,
                                                BufferMapRangeAccess rhs) {
    return magic_enum::bitwise_operators::operator^(lhs, rhs);
}

inline constexpr BufferMapRangeAccess operator|=(BufferMapRangeAccess lhs,
                                                 BufferMapRangeAccess rhs) {
    return magic_enum::bitwise_operators::operator|=(lhs, rhs);
}

inline constexpr BufferMapRangeAccess operator&=(BufferMapRangeAccess lhs,
                                                 BufferMapRangeAccess rhs) {
    return magic_enum::bitwise_operators::operator&=(lhs, rhs);
}

inline constexpr BufferMapRangeAccess operator^=(BufferMapRangeAccess lhs,
                                                 BufferMapRangeAccess rhs) {
    return magic_enum::bitwise_operators::operator^=(lhs, rhs);
}

std::string_view format_as(BufferMapRangeAccess value);

} // namespace gnev::gl

namespace magic_enum::customize {

template <>
struct enum_range<gnev::gl::BufferMapRangeAccess> {
    static constexpr bool is_flags = true;
};

} // namespace magic_enum::customize
