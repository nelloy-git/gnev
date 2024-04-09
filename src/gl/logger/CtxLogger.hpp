#pragma once

#include "util/Export.hpp"
#include "util/Logger.hpp"
#include "util/SrcLoc.hpp"

namespace gnev::gl {

class EXPORT CtxLogger {
    template <bool HasResult>
    static constexpr CtString FuncResFmt = CtString{" -> {}"}.repeat < HasResult ? 1
                                                                                 : 0 > ();

    template <std::size_t ArgsN, bool HasResult>
    static constexpr CtString FuncFmt{CtString{"{}("} +
                                      CtString{"{}"}.repeatSep<ArgsN>(CtString{", "}) +
                                      CtString{")"} + FuncResFmt<HasResult>};

    template <std::size_t ArgsN>
    static constexpr CtString PtrFmt{CtString{"{} -> "} +
                                     CtString{"{}"}.repeatSep<ArgsN>(CtString{", "})};

public:
    CtxLogger(const Logger& logger, const SrcLoc& src_loc)
        : logger{logger}
        , src_loc(src_loc) {}

    template <typename... Args>
    void Func(Args&&... args) const {
        static constexpr std::size_t ArgsN = sizeof...(Args);

        if (src_loc.non_void_return) {
            static constexpr auto Fmt = FuncFmt<ArgsN - 1, true>;
            logger.L3<Fmt>(src_loc.short_func_name, std::forward<Args>(args)...);
        } else {
            static constexpr auto Fmt = FuncFmt<ArgsN, false>;
            logger.L3<Fmt>(src_loc.short_func_name, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    void Ptr(const void* ptr, Args&&... args) const {
        static constexpr std::size_t ArgsN = sizeof...(Args);
        static constexpr auto Fmt = PtrFmt<ArgsN>;
        logger.L3<Fmt>(ptr, std::forward<Args>(args)...);
    }

private:
    const Logger& logger;
    const SrcLoc& src_loc;
};

} // namespace gnev::gl