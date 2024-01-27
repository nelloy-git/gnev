#pragma once

#include "util/CtString.hpp"
#include "util/Log.hpp"

namespace gnev::gl {

class CtxLog {
public:
    CtxLog(const CtString<>& method_name)
        : method_name(method_name) {}

    template <typename... Args>
    void log(Args&&... args) {
        static constexpr auto Fmt = getFmt<sizeof...(args)>();
        Log::template L3<Fmt>(method_name.to_string_view(), std::forward<Args>(args)...);
    }

    template <typename... Args>
    void logRes(Args&&... args) {
        static constexpr auto Fmt = getFmtRes<sizeof...(args) - 1>();
        Log::template L3<Fmt>(method_name.to_string_view(), std::forward<Args>(args)...);
    }

    template <typename... Args>
    void logPtr(const void* const ptr, Args&&... args) {
        static constexpr auto Fmt = getFmtPtr<sizeof...(args)>();
        Log::template L3<Fmt>(method_name.to_string_view(), ptr, std::forward<Args>(args)...);
    }

private:
    const CtString<>& method_name;

    template <std::size_t ArgsN>
    static consteval auto getFmt() {
        constexpr CtString Prefix{"{}("};
        constexpr CtString Body =
            CtStringRepeatSep<CtString{"{}"}, CtString{", "}, ArgsN>();
        constexpr CtString Suffix{")"};
        return CtStringConcat<Prefix, Body, Suffix>();
    }

    template <std::size_t ArgsN>
    static consteval auto getFmtRes() {
        constexpr CtString Prefix{"{}("};
        constexpr CtString Body =
            CtStringRepeatSep<CtString("{}"), CtString(", "), ArgsN>();
        constexpr CtString Suffix = {") -> {}"};
        return CtStringConcat<Prefix, Body, Suffix>();
    }

    template <std::size_t ArgsN>
    static consteval auto getFmtPtr() {
        constexpr CtString Prefix{"{} {} -> "};
        constexpr CtString Body =
            CtStringRepeatSep<CtString("{}"), CtString(", "), ArgsN>();
        return CtStringConcat<Prefix, Body>();
    }
};

} // namespace gnev::gl