#pragma once

#include "gl/Ctx.hpp"
#include "util/Logger.hpp"

namespace gnev::fmt {

struct Enum {
    static const char* const UNKNOWN;
    static constexpr bool PRINT_VALUE = false;
    static constexpr bool PRINT_NAME = true;

    enum class Group {
        None,
        DrawElements,
        GetProgramResourceIndex
    };

    Enum(GLenum value, Group group = Group::None);
    GLenum value;
    Group group;

    friend std::ostream& operator<<(std::ostream& os, const Enum& obj) {
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

private:
    const char* searchGroupNone() const;
    const char* searchGroupDrawElements() const;
    const char* searchGetProgramResourceIndex() const;
};

} // namespace gnev

template <>
struct fmtquill::formatter<gnev::fmt::Enum> : ostream_formatter {};