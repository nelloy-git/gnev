#pragma once

#include "util/CtString.hpp"
#include "util/Log.hpp"

namespace gnev::gl {

class HandlerLog {
public:
    HandlerLog(const std::string_view& class_name,
               const std::string_view& method_name,
               unsigned int handle)
        : class_name(class_name)
        , method_name(method_name)
        , handle(handle) {}

    template <typename... Args>
    void log(Args&&... args) {
        static constexpr auto Fmt = getFmt<sizeof...(args)>();
        Log::template L2<Fmt>(class_name, handle, method_name, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void logRes(Args&&... args) {
        static constexpr auto Fmt = getFmtRes<sizeof...(args) - 1>();
        Log::template L2<Fmt>(class_name, handle, method_name, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void logPtr(const void* const ptr, Args&&... args) {
        static constexpr auto Fmt = getFmtPtr<sizeof...(args)>();
        Log::template L2<Fmt>(class_name, handle, method_name, ptr, std::forward<Args>(args)...);
    }

private:
    const std::string_view class_name;
    const std::string_view method_name;
    const unsigned int handle;

    template <std::size_t ArgsN, std::size_t TabsN = 0>
    static consteval auto getFmt() {
        constexpr CtString Prefix{"{}<{}>::{}("};
        constexpr CtString Body =
            CtStringRepeatSep<CtString{"{}"}, CtString{", "}, ArgsN>();
        constexpr CtString Suffix{")"};
        return CtStringConcat<Prefix, Body, Suffix>();
    }

    template <std::size_t ArgsN>
    static consteval auto getFmtRes() {
        constexpr CtString Prefix{"{}<{}>::{}("};
        constexpr CtString Body =
            CtStringRepeatSep<CtString("{}"), CtString(", "), ArgsN>();
        constexpr CtString Suffix = {") -> {}"};
        return CtStringConcat<Prefix, Body, Suffix>();
    }

    template <std::size_t ArgsN>
    static consteval auto getFmtPtr() {
        constexpr CtString Prefix{"{}<{}>::{} {} -> "};
        constexpr CtString Body =
            CtStringRepeatSep<CtString("{}"), CtString(", "), ArgsN>();
        return CtStringConcat<Prefix, Body>();
    }

    template<std::size_t ArgsN>
    static consteval auto getFmtError(){
        constexpr CtString Prefix{"{}<{}>::{}("};
        constexpr CtString Body =
            CtStringRepeatSep<CtString("{}"), CtString(", "), ArgsN>();
        constexpr CtString Suffix = {") -> {}"};
        return CtStringConcat<Prefix, Body, Suffix>();
    }
};

} // namespace gnev::gl