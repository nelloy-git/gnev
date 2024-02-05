#pragma once

#include "util/CtString.hpp"
#include "util/Export.hpp"
#include "util/Log.hpp"
#include "util/SrcLoc.hpp"

namespace gnev::gl {

class EXPORT HandlerLog {

private:
    const SrcLoc& src_loc;
    const unsigned int handle;

    static constexpr CtString Signature{"{}<{}>::{}"};
    template <std::size_t ArgsN>
    static constexpr CtString ArgsCS =
        CtStringConcat<CtString{"("},
                       CtStringRepeatSep<CtString{"{}"}, CtString{", "}, ArgsN>(),
                       CtString{")"}>();
    template <std::size_t ArgsN>
    static constexpr CtString ArgsResCS = CtStringConcat<ArgsCS<ArgsN>>();
    static constexpr auto L = CtStringConcatLength<CtString{" {} -> {}"}>();
    static constexpr CtString PtrCS = CtStringConcat<CtString{" {} -> {}"}>();
    static constexpr CtString MsgCS = CtStringConcat<CtString{" => \"{}\""}>();

public:
    HandlerLog(const SrcLoc& src_loc, unsigned int handle)
        : src_loc(src_loc)
        , handle(handle) {}

    template<typename... Args>
    void Func(Args&&... args) const {
        if consteval
    }

    template <CtString Suffix = CtStringEmpty, typename... Args>
    void L2(Args&&... args) {
        

        if constexpr (Suffix == CtStringEmpty) {
            static constexpr auto Fmt =
                CtStringConcat<Signature, CtString{" "}, Log::LIST<sizeof...(args)>>();
            Log::template L2<Fmt>(src_loc.std_function_name,
                                  handle,
                                  src_loc.result_name,
                                  std::forward<Args>(args)...);
        } else {
            static constexpr auto Fmt = CtStringConcat<Signature, Suffix>();
            Log::template L2<Fmt>(src_loc.result_name,
                                  handle,
                                  src_loc.short_func_name,
                                  std::forward<Args>(args)...);
        }
    }
};

} // namespace gnev::gl