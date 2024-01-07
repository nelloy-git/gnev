#include "gl/fmt/BitFlags.hpp"

#include "magic_enum/magic_enum_flags.hpp"

namespace {

enum class glClearBit : GLbitfield {
    COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT,
    DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT,
    STENCIL_BUFFER_BIT = GL_STENCIL_BUFFER_BIT,
};

enum class glBufferStorageBit : GLbitfield {
    DYNAMIC_STORAGE_BIT = GL_DYNAMIC_STORAGE_BIT,
    MAP_READ_BIT = GL_MAP_READ_BIT,
    MAP_WRITE_BIT = GL_MAP_WRITE_BIT,
    MAP_PERSISTENT_BIT = GL_MAP_PERSISTENT_BIT,
    MAP_COHERENT_BIT = GL_MAP_COHERENT_BIT,
    CLIENT_STORAGE_BIT = GL_CLIENT_STORAGE_BIT
};

} // namespace

template <>
struct magic_enum::customize::enum_range<glClearBit> {
    static constexpr bool is_flags = true;
};

template <>
struct magic_enum::customize::enum_range<glBufferStorageBit> {
    static constexpr bool is_flags = true;
};

namespace gnev::fmt {

BitFlags::BitFlags(GLbitfield value, Group group)
    : value(value)
    , group(group) {}

std::string BitFlags::toString() const {
    switch (group) {
        using enum Group;
    case glClear:
        return toString_glClear();
    case glBufferStorage:
        return toString_glBufferStorage();
    default:
        return "UNKNOWN_GROUP";
    }
}

std::string BitFlags::toString_glClear() const {
    auto enum_flags = magic_enum::enum_flags_cast<glClearBit>(value);
    return enum_flags.has_value() ? magic_enum::enum_flags_name(enum_flags.value())
                                  : "UNKNOWN_FLAGS";
}

std::string BitFlags::toString_glBufferStorage() const {
    auto enum_flags = magic_enum::enum_flags_cast<glBufferStorageBit>(value);
    return enum_flags.has_value() ? magic_enum::enum_flags_name(enum_flags.value())
                                  : "UNKNOWN_FLAGS";
}

} // namespace gnev::fmt