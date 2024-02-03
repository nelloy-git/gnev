#pragma once

#include "gl/Ctx.hpp"
#include "util/Log.hpp"

namespace gnev::fmt {

struct String {
    String(const std::string& value)
        : value{value} {};
    String(const char* cstr)
        : value{cstr} {};
    String(const String& other)
        : value{other.value} {};

    std::string value;

    friend std::ostream& operator<<(std::ostream& os, const String& obj) {
        return os << "\"" << obj.value.c_str() << "\"";
    }
};

} // namespace gnev::fmt

/** Registered as safe to copy **/
namespace quill {
template <>
struct copy_loggable<gnev::fmt::String> : std::true_type {};
} // namespace quill

template <>
struct fmtquill::formatter<gnev::fmt::String> : ostream_formatter {};
