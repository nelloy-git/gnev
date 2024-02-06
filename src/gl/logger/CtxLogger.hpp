#pragma once

#include "util/Export.hpp"
#include "util/Logger.hpp"
#include "util/SrcLoc.hpp"

namespace gnev::gl {

class EXPORT CtxLogger {
    static constexpr CtString SignatureCS{"{}"};
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

    template <std::size_t ArgsN>
    static constexpr CtString PtrCs = CtStringConcat<CtString{"{} -> "}, ArgsCS<ArgsN>>();

    static constexpr CtString MsgFmt =
        CtStringConcat<SignatureCS, CtString{" => \"{}\""}>();

public:
    CtxLogger(const SrcLoc& src_loc)
        : src_loc(src_loc) {}

    template <typename... Args>
    void Func(Args&&... args) const {
        static constexpr std::size_t ArgsN = sizeof...(Args);

        if (src_loc.non_void_return) {
            static constexpr auto Fmt = Pattern<ArgsN, true>;
            Logger::template L3<Fmt>(src_loc.short_func_name,
                                     std::forward<Args>(args)...);
        } else {
            static constexpr auto Fmt = Pattern<ArgsN, false>;
            Logger::template L3<Fmt>(src_loc.short_func_name,
                                     std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    void Ptr(const void* ptr, Args&&... args) const {
        static constexpr std::size_t ArgsN = sizeof...(Args);
        static constexpr auto Fmt = PtrCs<ArgsN>;
        Logger::template L3<Fmt>(ptr, std::forward<Args>(args)...);
    }

private:
    const SrcLoc& src_loc;
};

} // namespace gnev::gl