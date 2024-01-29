#pragma once

#include "util/CtString.hpp"
#include "util/Export.hpp"
#include "util/Log.hpp"

namespace gnev::gl {

class EXPORT CtxLog {

public:
    CtxLog(const CtString<>& class_name = getClassName(),
           const CtString<>& method_name = getMethodName())
        : class_name(class_name)
        , method_name(method_name) {}

    CtxLog(const CtxLog&) = delete;
    CtxLog(CtxLog&&) = delete;

    template <typename... Args>
    void L3(Args&&... args) {
        static constexpr auto Fmt = getL3Fmt<sizeof...(args)>();
        Log::template L3<Fmt>(method_name.to_string_view(), std::forward<Args>(args)...);
    }

    template <typename... Args>
    void L3res(Args&&... args) {
        static constexpr auto Fmt = getL3resFmt<sizeof...(args) - 1>();
        Log::template L3<Fmt>(method_name.to_string_view(), std::forward<Args>(args)...);
    }

    template <typename... Args>
    void L3ptr(const void* const ptr, Args&&... args) {
        static constexpr auto Fmt = getL3ptrFmt<sizeof...(args)>();
        Log::template L3<Fmt>(method_name.to_string_view(),
                              ptr,
                              std::forward<Args>(args)...);
    }

    template <CtString<> Fmt = CtString<>{""}, typename... Args>
    void INFO(Args&&... args) const {
        if constexpr (Fmt.length <= 1) {
            static constexpr auto DefaultFmt = getMsgFmt<sizeof...(Args)>();
            INFO_helper<DefaultFmt>(std::forward<Args>(args)...);
        } else {
            static constexpr auto MergedFmt = getMsgFmt<Fmt>();
            INFO_helper<MergedFmt>(std::forward<Args>(args)...);
        }
    }

    template <CtString<> Fmt = CtString<>{""}, typename... Args>
    void DEBUG(Args&&... args) const {
        if constexpr (Fmt.length <= 1) {
            static constexpr auto DefaultFmt = getMsgFmt<sizeof...(Args)>();
            DEBUG_helper<DefaultFmt>(std::forward<Args>(args)...);
        } else {
            DEBUG_helper<Fmt>(std::forward<Args>(args)...);
        }
    }

    template <CtString<> Fmt = CtString<>{""}, typename... Args>
    void ERROR(Args&&... args) const {
        if constexpr (Fmt.length <= 1) {
            static constexpr auto DefaultFmt = getMsgFmt<sizeof...(Args)>();
            ERROR_helper<DefaultFmt>(std::forward<Args>(args)...);
        } else {
            ERROR_helper<Fmt>(std::forward<Args>(args)...);
        }
    }

private:
    const CtString<>& class_name;
    const CtString<>& method_name;

    template <std::size_t ArgsN>
    static consteval auto getL3Fmt() {
        constexpr CtString Prefix{"{}("};
        constexpr CtString Body =
            CtStringRepeatSep<CtString{"{}"}, CtString{", "}, ArgsN>();
        constexpr CtString Suffix{")"};
        return CtStringConcat<Prefix, Body, Suffix>();
    }

    template <std::size_t ArgsN>
    static consteval auto getL3resFmt() {
        constexpr CtString Prefix{"{}("};
        constexpr CtString Body =
            CtStringRepeatSep<CtString("{}"), CtString(", "), ArgsN>();
        constexpr CtString Suffix = {") -> {}"};
        return CtStringConcat<Prefix, Body, Suffix>();
    }

    template <std::size_t ArgsN>
    static consteval auto getL3ptrFmt() {
        constexpr CtString Prefix{"{} {} -> "};
        constexpr CtString Body =
            CtStringRepeatSep<CtString("{}"), CtString(", "), ArgsN>();
        return CtStringConcat<Prefix, Body>();
    }

    template <CtString<> Fmt, typename... Args>
    void INFO_helper(Args&&... args) const {
        Log::template INFO<Fmt>(class_name.to_string_view(),
                                method_name.to_string_view(),
                                std::forward<Args>(args)...);
    }

    template <CtString<> Fmt, typename... Args>
    void DEBUG_helper(Args&&... args) const {
        Log::template DEBUG<Fmt>(class_name.to_string_view(),
                                 method_name.to_string_view(),
                                 std::forward<Args>(args)...);
    }

    template <CtString<> Fmt, typename... Args>
    void ERROR_helper(Args&&... args) const {
        Log::template ERROR<Fmt>(class_name.to_string_view(),
                                 method_name.to_string_view(),
                                 std::forward<Args>(args)...);
    }

    static consteval auto getMsgPref() { return CtString{"{}::{} => "}; }

    static consteval auto getMsgSuff() { return CtString{""}; }

    template <std::size_t ArgsN>
    static consteval auto getMsgFmt() {
        constexpr CtString Prefix = getMsgPref();
        constexpr CtString Body =
            CtStringRepeatSep<CtString{"{}"}, CtString{", "}, ArgsN>();
        constexpr CtString Suffix = getMsgSuff();
        return CtStringConcat<Prefix, Body, Suffix>();
    }

    template <CtString<> Fmt>
    static consteval auto getMsgFmt() {
        constexpr CtString Prefix = getMsgPref();
        constexpr CtString Body = CtStringOptimize<Fmt>();
        constexpr CtString Suffix = getMsgSuff();
        return CtStringConcat<Prefix, Body, Suffix>();
    }
};

} // namespace gnev::gl