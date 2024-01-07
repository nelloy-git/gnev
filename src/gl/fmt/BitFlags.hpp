#pragma once

#include "gl/Ctx.hpp"
#include "util/Log.hpp"

namespace gnev::fmt {

struct BitFlags {
    using BitSet = std::bitset<std::numeric_limits<GLbitfield>::digits>;
    static constexpr bool PRINT_VALUE = false;
    static constexpr bool PRINT_NAME = true;

    enum class Group {
        glClear,
        glBufferStorage
    };

    BitFlags(GLbitfield value, Group group);

    GLbitfield value;
    Group group;

    std::string toString() const;

    friend std::ostream& operator<<(std::ostream& os, const BitFlags& flags) {
        if constexpr (PRINT_VALUE and PRINT_NAME) {
            os << flags.toString().data() << "(" << BitSet(flags.value) << ")";
        } else if constexpr (PRINT_VALUE) {
            os << BitSet(flags.value);
        } else if constexpr (PRINT_NAME) {
            os << flags.toString().data();
        }
        return os;
    }

private:
    std::string toString_glClear() const;
    std::string toString_glBufferStorage() const;

};

} // namespace gnev::fmt

template <>
struct fmtquill::formatter<gnev::fmt::BitFlags> : ostream_formatter {};