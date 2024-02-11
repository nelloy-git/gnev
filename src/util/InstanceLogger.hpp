#pragma once

#include "util/Export.hpp"
#include "util/Logger.hpp"
#include "util/SrcLoc.hpp"

namespace gnev {

class EXPORT InstanceLogger {
    static constexpr CtString SignatureFmt{"{}::{} "};
    static constexpr CtString SignatureFmtWithId{"{}<{}>::{} "};

public:
    InstanceLogger(const SrcLoc& src_loc = SrcLoc::Current())
        : src_loc{src_loc}
        , id{std::nullopt} {}

    InstanceLogger(const SrcLoc& src_loc, std::size_t id)
        : src_loc{src_loc}
        , id{id} {}

    template <LogLevel Lvl, CtString FmtSuff, typename... Args>
    void Log(Args&&... args) const {
        if (id.has_value()) {
            static constexpr auto Fmt = CtStringConcat<SignatureFmtWithId, FmtSuff>();
            LOG<Lvl, Fmt>(src_loc.class_name,
                          id.value(),
                          src_loc.short_func_name,
                          std::forward<Args>(args)...);
        } else {
            static constexpr auto Fmt = CtStringConcat<SignatureFmt, FmtSuff>();
            LOG<Lvl, Fmt>(src_loc.class_name,
                          src_loc.short_func_name,
                          std::forward<Args>(args)...);
        }
    }

private:
    const SrcLoc& src_loc;
    const std::optional<std::size_t> id;
};

} // namespace gnev