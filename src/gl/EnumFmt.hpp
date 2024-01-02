#pragma once

#include "gl/Ctx.hpp"
#include "util/Log.hpp"

namespace gnev {

struct EnumFmt {
    static const char* const UNKNOWN;
    static constexpr bool PRINT_VALUE = false;
    static constexpr bool PRINT_NAME = true;

    enum class Group {
        None,
        DrawElements
    };

    EnumFmt(GLenum value, Group group = Group::None);
    GLenum value;
    Group group;

    friend std::ostream& operator<<(std::ostream& os, const EnumFmt& obj) {
        if constexpr (PRINT_VALUE and PRINT_NAME) {
            os << obj.searchMacroName() << "(" << obj.value << ")";
        } else if constexpr (PRINT_VALUE) {
            os << obj.value;
        } else if constexpr (PRINT_NAME) {
            os << obj.searchMacroName();
        }
        return os;
    }

    const char* searchMacroName() const;

    const char* searchGroupNone() const;
    const char* searchGroupDrawElements() const;
};

} // namespace gnev

template <>
struct fmtquill::formatter<gnev::EnumFmt> : ostream_formatter {};

template <>
struct fmtquill::formatter<std::bitset<std::numeric_limits<GLbitfield>::digits>> : ostream_formatter {};