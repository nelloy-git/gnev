#pragma once

#include "gl/logger/CtxLogger.hpp"

namespace gnev::gl {

class EXPORT HandlerLogger : public CtxLogger {
public:
    template <std::size_t ArgsN, bool HasResult>
    static constexpr CtString HandleFuncFmt =
        CtString{"{}<{}>::"} + FuncFmt<ArgsN, HasResult>;

    HandlerLogger(CtxLogger&& logger, unsigned int handle)
        : CtxLogger{std::move(logger)}
        , handle{handle} {}

    template <LogLevel Level, typename... Args>
    void logFunc(Args&&... args) const {
        static constexpr std::size_t ArgsN = sizeof...(Args);

        if (src_loc.non_void_return) {
            static constexpr CtString Fmt =
                HandleFuncFmt<(ArgsN > 0 ? ArgsN - 1 : 0), true>;
            logger.log<Level, Fmt>(src_loc.class_name,
                                   handle,
                                   src_loc.short_func_name,
                                   std::forward<Args>(args)...);
        } else {
            static constexpr CtString Fmt = HandleFuncFmt<ArgsN, false>;
            logger.log<Level, Fmt>(src_loc.class_name,
                                   handle,
                                   src_loc.short_func_name,
                                   std::forward<Args>(args)...);
        }
    }

    static constexpr CtString MsgFmt = CtString{"{}<{}>::{} => \"{}\""};

    template <LogLevel Level>
    void logMsg(auto&& msg) const {
        log<Level, MsgFmt>(src_loc.class_name,
                           handle,
                           src_loc.short_func_name,
                           std::forward<decltype(msg)>(msg));
    }

private:
    const unsigned int handle;
};

} // namespace gnev::gl