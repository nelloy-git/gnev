#pragma once

#include "util/CtString.hpp"
#include "util/Export.hpp"
#include "util/Logger.hpp"
#include "util/SrcLoc.hpp"

namespace gnev::gl {

// TODO move to gl/logger/...
class EXPORT CtxLog {

private:
    const SrcLoc& src_loc;

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

    template <CtStrDef Fmt, typename... Args>
    void INFO_helper(Args&&... args) const {
        Logger::template INFO<Fmt>(src_loc.class_name,
                                   src_loc.short_func_name,
                                   std::forward<Args>(args)...);
    }

    template <CtStrDef Fmt, typename... Args>
    void DEBUG_helper(Args&&... args) const {
        Logger::template DEBUG<Fmt>(src_loc.class_name,
                                    src_loc.short_func_name,
                                    std::forward<Args>(args)...);
    }

    template <CtStrDef Fmt, typename... Args>
    void ERROR_helper(Args&&... args) const {
        Logger::template ERROR<Fmt>(src_loc.class_name,
                                    src_loc.short_func_name,
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

    template <CtStrDef Fmt>
    static consteval auto getMsgFmt() {
        constexpr CtString Prefix = getMsgPref();
        constexpr CtString Body = CtStringOptimize<Fmt>();
        constexpr CtString Suffix = getMsgSuff();
        return CtStringConcat<Prefix, Body, Suffix>();
    }

public:
    CtxLog(const SrcLoc& src_loc = SrcLoc::Current())
        : src_loc(src_loc) {}

    CtxLog(const CtxLog&) = delete;
    CtxLog(CtxLog&&) = delete;

    template <typename... Args>
    void L3(Args&&... args) {
        static constexpr auto Fmt = getL3Fmt<sizeof...(args)>();
        Logger::template L3<Fmt>(src_loc.short_func_name, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void L3res(Args&&... args) {
        static constexpr auto Fmt = getL3resFmt<sizeof...(args) - 1>();
        Logger::template L3<Fmt>(src_loc.short_func_name, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void L3ptr(const void* const ptr, Args&&... args) {
        static constexpr auto Fmt = getL3ptrFmt<sizeof...(args)>();
        Logger::template L3<Fmt>(src_loc.short_func_name,
                                 ptr,
                                 std::forward<Args>(args)...);
    }
};

} // namespace gnev::gl