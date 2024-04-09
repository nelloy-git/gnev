#pragma once

#include "util/Export.hpp"
#include "util/Logger.hpp"
#include "util/SrcLoc.hpp"

namespace gnev::gl {

class EXPORT HandlerLogger {
    static constexpr CtString SignatureCS{"{}<{}>::{}"};
    template <std::size_t ArgsN>
    static constexpr CtString ArgsCS =
        CtString{"("} + CtStringRepeatSep<CtString{"{}"}, CtString{", "}, ArgsN>() +
        CtString{")"};
    template <bool HasRes>
    static constexpr CtString ResCS = []() {
        if constexpr (HasRes)
            return CtString{" -> {}"};
        else
            return CtString{""};
    }();

    template <std::size_t ArgsN, bool HasRes>
    static constexpr CtString Pattern = []() {
        if constexpr (ArgsN >= 1) {
            return SignatureCS + ArgsCS < HasRes ? ArgsN - 1 : ArgsN > +ResCS<HasRes>;
        } else {
            return SignatureCS + ArgsCS<0> + ResCS<false>;
        }
    }();

    static constexpr CtString MsgFmt = SignatureCS + CtString{" => \"{}\""};

public:
    HandlerLogger(const Logger& logger, unsigned int handle, const SrcLoc& src_loc)
        : logger{logger}
        , handle{handle}
        , src_loc{src_loc} {}

    template <typename... Args>
    void Func(Args&&... args) const {
        // static constexpr std::size_t ArgsN = sizeof...(Args);
        // static constexpr auto Fmt = Pattern<ArgsN, false>;
        // logger.L2<Fmt>(src_loc.class_name,
        //                handle,
        //                src_loc.short_func_name,
        //                std::forward<Args>(args)...);
    }

    void DEBUG(auto&& msg) const {
        // static constexpr auto Fmt = MsgFmt;
        // logger.DEBUG<Fmt>(src_loc.class_name,
        //                   handle,
        //                   src_loc.short_func_name,
        //                   std::forward<decltype(msg)>(msg));
    }

    void INFO(auto&& msg) const {
        // static constexpr auto Fmt = MsgFmt;
        // logger.INFO<Fmt>(src_loc.class_name,
        //                  handle,
        //                  src_loc.short_func_name,
        //                  std::forward<decltype(msg)>(msg));
    }

    void WARNING(auto&& msg) const {
        // static constexpr auto Fmt = MsgFmt;
        // logger.WARNING<Fmt>(src_loc.class_name,
        //                     handle,
        //                     src_loc.short_func_name,
        //                     std::forward<decltype(msg)>(msg));
    }

    void ERROR(auto&& msg) const {
        // static constexpr auto Fmt = MsgFmt;
        // logger.ERROR<Fmt>(src_loc.class_name,
        //                   handle,
        //                   src_loc.short_func_name,
        //                   std::forward<decltype(msg)>(msg));
    }

    void CRITICAL(auto&& msg) const {
        // static constexpr auto Fmt = MsgFmt;
        // logger.CRITICAL<Fmt>(src_loc.class_name,
        //                      handle,
        //                      src_loc.short_func_name,
        //                      std::forward<decltype(msg)>(msg));
    }

private:
    const Logger& logger;
    const unsigned int handle;
    const SrcLoc& src_loc;
};

} // namespace gnev::gl