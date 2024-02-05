#pragma once

#include <atomic>
#include <memory>

#include "quill/Quill.h"
#include "util/CtString.hpp"
#include "util/Export.hpp"

namespace gnev {

class EXPORT Logger {
public:
    static constexpr auto LOGGER_NAME = "gnev";

    template <std::size_t ArgsN>
    static constexpr CtString LIST =
        CtStringRepeatSep<CtString{"{}"}, CtString{", "}, ArgsN>();

    static void init();

    template <CtString Fmt = CtStringEmpty, typename... Args>
    static void L3(Args&&... args) {
        if constexpr (Fmt == CtStringEmpty) {
            static constexpr CtString DefaultFmt = LIST<sizeof...(Args)>;
            QUILL_LOG_TRACE_L3(quill_logger,
                               DefaultFmt.array.data(),
                               std::forward<Args>(args)...);
        } else {
            QUILL_LOG_TRACE_L3(quill_logger,
                               Fmt.array.data(),
                               std::forward<Args>(args)...);
        }
    }

    template <CtString Fmt = CtStringEmpty, typename... Args>
    static void L2(Args&&... args) {
        if constexpr (Fmt == CtStringEmpty) {
            static constexpr CtString DefaultFmt = LIST<sizeof...(Args)>;
            QUILL_LOG_TRACE_L2(quill_logger,
                               DefaultFmt.array.data(),
                               std::forward<Args>(args)...);
        } else {
            QUILL_LOG_TRACE_L2(quill_logger,
                               Fmt.array.data(),
                               std::forward<Args>(args)...);
        }
    }

    template <CtString Fmt = CtStringEmpty, typename... Args>
    static void L1(Args&&... args) {
        if constexpr (Fmt == CtStringEmpty) {
            static constexpr CtString DefaultFmt = LIST<sizeof...(Args)>;
            QUILL_LOG_TRACE_L1(quill_logger,
                               DefaultFmt.array.data(),
                               std::forward<Args>(args)...);
        } else {
            QUILL_LOG_TRACE_L1(quill_logger,
                               Fmt.array.data(),
                               std::forward<Args>(args)...);
        }
    }

    template <CtString Fmt = CtStringEmpty, typename... Args>
    static void DEBUG(Args&&... args) {
        if constexpr (Fmt == CtStringEmpty) {
            static constexpr CtString DefaultFmt = LIST<sizeof...(Args)>;
            QUILL_LOG_DEBUG(quill_logger,
                            DefaultFmt.array.data(),
                            std::forward<Args>(args)...);
        } else {
            QUILL_LOG_DEBUG(quill_logger, Fmt.array.data(), std::forward<Args>(args)...);
        }
    }

    template <CtString Fmt = CtStringEmpty, typename... Args>
    static void INFO(Args&&... args) {
        if constexpr (Fmt == CtStringEmpty) {
            static constexpr CtString DefaultFmt = LIST<sizeof...(Args)>;
            QUILL_LOG_INFO(quill_logger,
                           DefaultFmt.array.data(),
                           std::forward<Args>(args)...);
        } else {
            QUILL_LOG_INFO(quill_logger, Fmt.array.data(), std::forward<Args>(args)...);
        }
    }

    template <CtString Fmt = CtStringEmpty, typename... Args>
    static void WARNING(Args&&... args) {
        if constexpr (Fmt == CtStringEmpty) {
            static constexpr CtString DefaultFmt = LIST<sizeof...(Args)>;
            QUILL_LOG_WARNING(quill_logger,
                              DefaultFmt.array.data(),
                              std::forward<Args>(args)...);
        } else {
            QUILL_LOG_WARNING(quill_logger,
                              Fmt.array.data(),
                              std::forward<Args>(args)...);
        }
    }

    template <CtString Fmt = CtStringEmpty, typename... Args>
    static void ERROR(Args&&... args) {
        if constexpr (Fmt == CtStringEmpty) {
            static constexpr CtString DefaultFmt = LIST<sizeof...(Args)>;
            QUILL_LOG_ERROR(quill_logger,
                            DefaultFmt.array.data(),
                            std::forward<Args>(args)...);
        } else {
            QUILL_LOG_ERROR(quill_logger, Fmt.array.data(), std::forward<Args>(args)...);
        }
    }

    template <CtString Fmt = CtStringEmpty, typename... Args>
    static void CRITICAL(Args&&... args) {
        if constexpr (Fmt == CtStringEmpty) {
            static constexpr CtString DefaultFmt = LIST<sizeof...(Args)>;
            QUILL_LOG_CRITICAL(quill_logger,
                               DefaultFmt.array.data(),
                               std::forward<Args>(args)...);
        } else {
            QUILL_LOG_CRITICAL(quill_logger,
                               Fmt.array.data(),
                               std::forward<Args>(args)...);
        }
    }

private:
    Logger();

    static std::atomic<bool> inited;
    inline static quill::Logger* quill_logger{nullptr};
};

} // namespace gnev