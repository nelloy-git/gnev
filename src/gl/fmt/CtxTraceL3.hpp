#pragma once

#include "util/CtString.hpp"
#include "util/Log.hpp"

namespace gnev::gl {

struct CtxTraceL3 {
    template <std::size_t ArgsN>
    static consteval auto getPattern() {
        constexpr CtString Prefix{"{}("};
        constexpr CtString Suffix = {")"};
        constexpr CtString Body =
            CtStringRepeatSep<CtString("{}"), CtString(", "), ArgsN>();
        return CtStringConcat<Prefix, Body, Suffix>();
    }

    template <std::size_t ArgsN>
    static consteval auto getPatternRes() {
        constexpr CtString Prefix{"{}("};
        constexpr CtString Suffix = {") -> {}"};
        constexpr CtString Body =
            CtStringRepeatSep<CtString("{}"), CtString(", "), ArgsN>();
        return CtStringConcat<Prefix, Body, Suffix>();
    }

    template <std::size_t ArgsN>
    static consteval auto getPatternPtr() {
        constexpr CtString Prefix{"{} -> "};
        constexpr CtString Body =
            CtStringRepeatSep<CtString("{}"), CtString(", "), ArgsN>();
        return CtStringConcat<Prefix, Body>();
    }

    template <typename... Args>
    CtxTraceL3(const CtString<128>& method_name)
        : method_name(method_name) {}

    const CtString<128>& method_name;

    template <typename... Args>
    void log(Args&&... args) {
        if constexpr (QUILL_ACTIVE_LOG_LEVEL <= QUILL_LOG_LEVEL_TRACE_L3) {
            static constexpr auto Pattern = getPattern<sizeof...(args)>();
            static constexpr std::string_view PatternView = Pattern.to_string_view();
            GNEV_TRACE_L3(PatternView, method_name.to_string_view(), args...);
        }
    }

    template <typename... Args>
    void logRes(Args&&... args) {
        if constexpr (QUILL_ACTIVE_LOG_LEVEL <= QUILL_LOG_LEVEL_TRACE_L3) {
            static constexpr auto Pattern = getPatternRes<sizeof...(args) - 1>();
            static constexpr std::string_view PatternView = Pattern.to_string_view();
            GNEV_TRACE_L3(PatternView, method_name.to_string_view(), args...);
        }
    }

    template <typename... Args>
    void logPtr(const void* const ptr, Args&&... args) {
        if constexpr (QUILL_ACTIVE_LOG_LEVEL <= QUILL_LOG_LEVEL_TRACE_L3) {
            static constexpr auto Pattern = getPatternPtr<sizeof...(args)>();
            static constexpr std::string_view PatternView = Pattern.to_string_view();
            GNEV_TRACE_L3(PatternView, ptr, args...);
        }
    }
};

} // namespace gnev::gl