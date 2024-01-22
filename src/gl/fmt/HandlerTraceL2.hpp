#pragma once

#include "util/CtString.hpp"
#include "util/Log.hpp"

namespace gnev::gl {

struct HandlerTraceL2 {
    template <bool WithRes, std::size_t ArgsN>
    static consteval auto getPattern() {
        // constexpr CtString Prefix{"{}<{}>::{}("};
        // constexpr CtString Suffix = []() {
        //     if constexpr (WithRes) {
        //         return CtString(") -> {}");
        //     } else {
        //         return CtString(")");
        //     }
        // }();
        // constexpr CtString Body =
        //     CtStringRepeatSep<CtString("{}"), CtString(", "), ArgsN>();
        // return CtStringConcat<Prefix, Body, Suffix>();
    }

    HandlerTraceL2(){}

    // const unsigned int handle;
    // const CtString<128>& class_name;
    // const CtString<128>& method_name;

    template <typename... Args>
    void log(Args&&... args) {
        if constexpr (QUILL_ACTIVE_LOG_LEVEL <= QUILL_LOG_LEVEL_TRACE_L2) {
            // static constexpr auto Pattern = getPattern<false, sizeof...(args)>();
            // static constexpr std::string_view PatternView = Pattern.to_string_view();
            // GNEV_TRACE_L2(PatternView,
            //               class_name.to_string_view(),
            //               handle,
            //               method_name.to_string_view(),
            //               args...);
        }
    }

    template <typename... Args>
    void logRes(Args&&... args) {
        if constexpr (QUILL_ACTIVE_LOG_LEVEL <= QUILL_LOG_LEVEL_TRACE_L2) {
            // static constexpr auto Pattern = getPattern<true, sizeof...(args) - 1>();
            // static constexpr std::string_view PatternView = Pattern.to_string_view();
            // GNEV_TRACE_L2(PatternView,
            //               class_name.to_string_view(),
            //               handle,
            //               method_name.to_string_view(),
            //               args...);
        }
    }
};

} // namespace gnev