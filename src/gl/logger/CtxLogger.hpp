#pragma once

#include "util/Export.hpp"
#include "util/Logger.hpp"
#include "util/SrcLoc.hpp"

namespace gnev::gl {

class EXPORT CtxLogger {
public:
    template <bool HasResult>
    static constexpr CtString FuncResFmt = CtString{" -> {}"}.repeat < HasResult ? 1
                                                                                 : 0 > ();

    template <std::size_t ArgsN, bool HasResult>
    static constexpr CtString FuncFmt{CtString{"{}("} +
                                      CtString{"{}"}.repeatSep<ArgsN>(CtString{", "}) +
                                      CtString{")"} + FuncResFmt<HasResult>};

    CtxLogger(const Logger& logger, const SrcLoc& src_loc)
        : logger{logger}
        , src_loc(src_loc) {}

    template <LogLevel Level, CtString Fmt, typename... Args>
    inline void log(Args&&... args) const {
        logger.log<Level, Fmt>(std::forward<Args>(args)...);
    }

    template <LogLevel Level, typename... Args>
    void logFunc(Args&&... args) const {
        static constexpr std::size_t ArgsN = sizeof...(Args);

        if (src_loc.non_void_return) {
            static constexpr auto Fmt = FuncFmt<(ArgsN > 0 ? ArgsN - 1 : 0), true>;
            logger.log<Level, Fmt>(src_loc.short_func_name, std::forward<Args>(args)...);
        } else {
            static constexpr auto Fmt = FuncFmt<ArgsN, false>;
            logger.log<Level, Fmt>(src_loc.short_func_name, std::forward<Args>(args)...);
        }
    }

    template <std::size_t ArgsN>
    static constexpr CtString PtrFmt{CtString{"{} -> "} +
                                     CtString{"{}"}.repeatSep<ArgsN>(CtString{", "})};

    template <LogLevel Level, typename... Args>
    void logPtr(const void* ptr, Args&&... args) const {
        static constexpr std::size_t ArgsN = sizeof...(Args);
        static constexpr auto Fmt = PtrFmt<ArgsN>;
        logger.log<Level, Fmt>(ptr, std::forward<Args>(args)...);
    }

protected:
    const Logger& logger;
    const SrcLoc& src_loc;
};

} // namespace gnev::gl