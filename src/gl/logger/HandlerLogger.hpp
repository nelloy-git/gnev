#pragma once

#include "util/Export.hpp"
#include "util/Logger.hpp"
#include "util/SrcLoc.hpp"

namespace gnev::gl {

class EXPORT HandlerLogger {
    static constexpr CtString SignatureCS{"{}<{}>::{}"};
    template <std::size_t ArgsN>
    static constexpr CtString ArgsCS =
        CtStringConcat<CtString{"("},
                       CtStringRepeatSep<CtString{"{}"}, CtString{", "}, ArgsN>(),
                       CtString{")"}>();
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
            return CtStringConcat<SignatureCS,
                                  ArgsCS<HasRes ? ArgsN - 1 : ArgsN>,
                                  ResCS<HasRes>>();
        } else {
            return CtStringConcat<SignatureCS, ArgsCS<0>, ResCS<false>>();
        }
    }();

    static constexpr CtString MsgFmt =
        CtStringConcat<SignatureCS, CtString{" => \"{}\""}>();

public:
    HandlerLogger(const SrcLoc& src_loc, unsigned int handle)
        : src_loc(src_loc)
        , handle(handle) {}

    template <typename... Args>
    void Func(Args&&... args) const {
        static constexpr std::size_t ArgsN = sizeof...(Args);
        static constexpr auto Fmt = Pattern<ArgsN, false>;
        Logger::template L2<Fmt>(src_loc.class_name,
                                 handle,
                                 src_loc.short_func_name,
                                 std::forward<Args>(args)...);
    }

    void DEBUG(auto&& msg) const {
        static constexpr auto Fmt = MsgFmt;
        Logger::DEBUG<Fmt>(src_loc.class_name,
                           handle,
                           src_loc.short_func_name,
                           std::forward<decltype(msg)>(msg));
    }

    void INFO(auto&& msg) const {
        static constexpr auto Fmt = MsgFmt;
        Logger::INFO<Fmt>(src_loc.class_name,
                          handle,
                          src_loc.short_func_name,
                          std::forward<decltype(msg)>(msg));
    }

    void WARNING(auto&& msg) const {
        static constexpr auto Fmt = MsgFmt;
        Logger::WARNING<Fmt>(src_loc.class_name,
                             handle,
                             src_loc.short_func_name,
                             std::forward<decltype(msg)>(msg));
    }

    void ERROR(auto&& msg) const {
        static constexpr auto Fmt = MsgFmt;
        Logger::ERROR<Fmt>(src_loc.class_name,
                           handle,
                           src_loc.short_func_name,
                           std::forward<decltype(msg)>(msg));
    }

    void CRITICAL(auto&& msg) const {
        static constexpr auto Fmt = MsgFmt;
        Logger::CRITICAL<Fmt>(src_loc.class_name,
                              handle,
                              src_loc.short_func_name,
                              std::forward<decltype(msg)>(msg));
    }

private:
    const SrcLoc& src_loc;
    const unsigned int handle;
};

} // namespace gnev::gl