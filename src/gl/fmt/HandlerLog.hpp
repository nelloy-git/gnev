#pragma once

#include "util/CtString.hpp"
#include "util/Log.hpp"

namespace gnev::gl {

class HandlerLog {
public:
    HandlerLog(const CtString<>& class_name,
               const CtString<>& method_name,
               unsigned int handle)
        : class_name(class_name)
        , method_name(method_name)
        , handle(handle) {}

    template <typename... Args>
    void L2(Args&&... args) {
        static constexpr auto Fmt = getL2Fmt<sizeof...(args)>();
        Log::template L2<Fmt>(class_name.to_string_view(),
                              handle,
                              method_name.to_string_view(),
                              std::forward<Args>(args)...);
    }

    template <typename... Args>
    void L2res(Args&&... args) {
        static constexpr auto Fmt = getL2resFmt<sizeof...(args) - 1>();
        Log::template L2<Fmt>(class_name.to_string_view(),
                              handle,
                              method_name.to_string_view(),
                              std::forward<Args>(args)...);
    }

    template <typename... Args>
    void L2ptr(const void* const ptr, Args&&... args) {
        static constexpr auto Fmt = getL2ptrFmt<sizeof...(args)>();
        Log::template L2<Fmt>(class_name.to_string_view(),
                              handle,
                              method_name.to_string_view(),
                              ptr,
                              std::forward<Args>(args)...);
    }

    void Error(auto&& msg) {
        static constexpr auto Fmt = getErrorFmt();
        Log::template L2<Fmt>(class_name.to_string_view(),
                              handle,
                              method_name.to_string_view(),
                              std::forward<decltype(msg)>(msg));
    }

private:
    const CtString<>& class_name;
    const CtString<>& method_name;
    const unsigned int handle;

    template <std::size_t ArgsN, std::size_t TabsN = 0>
    static consteval auto getL2Fmt() {
        constexpr CtString Prefix{"{}<{}>::{}("};
        constexpr CtString Body =
            CtStringRepeatSep<CtString{"{}"}, CtString{", "}, ArgsN>();
        constexpr CtString Suffix{")"};
        return CtStringConcat<Prefix, Body, Suffix>();
    }

    template <std::size_t ArgsN>
    static consteval auto getL2resFmt() {
        constexpr CtString Prefix{"{}<{}>::{}("};
        constexpr CtString Body =
            CtStringRepeatSep<CtString("{}"), CtString(", "), ArgsN>();
        constexpr CtString Suffix = {") -> {}"};
        return CtStringConcat<Prefix, Body, Suffix>();
    }

    template <std::size_t ArgsN>
    static consteval auto getL2ptrFmt() {
        constexpr CtString Prefix{"{}<{}>::{} {} -> "};
        constexpr CtString Body =
            CtStringRepeatSep<CtString("{}"), CtString(", "), ArgsN>();
        return CtStringConcat<Prefix, Body>();
    }

    static consteval auto getErrorFmt() { return CtString{"{}<{}>::{} {}"}; }
};

} // namespace gnev::gl