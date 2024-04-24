#pragma once

#include "Ctx.hpp"
#include "util/Logger.hpp"

namespace gnev::fmt {

struct CharPtr {
    CharPtr(const char* const value)
        : value(value){};
    const char* const value;

    friend std::ostream& operator<<(std::ostream& os, const CharPtr& obj) {
        return os << "\"" << obj.value << "\"";
    }
};

} // namespace gnev::fmt

template <>
struct fmtquill::formatter<gnev::fmt::CharPtr> : ostream_formatter {};
