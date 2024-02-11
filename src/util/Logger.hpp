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

    static void init();

    template <CtString Fmt, typename... Args>
    static void L3(Args&&... args) {
        QUILL_LOG_TRACE_L3(quill_logger, Fmt.array.data(), std::forward<Args>(args)...);
    }

    template <CtString Fmt, typename... Args>
    static void L2(Args&&... args) {
        QUILL_LOG_TRACE_L2(quill_logger, Fmt.array.data(), std::forward<Args>(args)...);
    }

    template <CtString Fmt, typename... Args>
    static void L1(Args&&... args) {
        QUILL_LOG_TRACE_L1(quill_logger, Fmt.array.data(), std::forward<Args>(args)...);
    }

    template <CtString Fmt, typename... Args>
    static void DEBUG(Args&&... args) {
        QUILL_LOG_DEBUG(quill_logger, Fmt.array.data(), std::forward<Args>(args)...);
    }

    template <CtString Fmt, typename... Args>
    static void INFO(Args&&... args) {
        QUILL_LOG_INFO(quill_logger, Fmt.array.data(), std::forward<Args>(args)...);
    }

    template <CtString Fmt, typename... Args>
    static void WARNING(Args&&... args) {
        QUILL_LOG_WARNING(quill_logger, Fmt.array.data(), std::forward<Args>(args)...);
    }

    template <CtString Fmt, typename... Args>
    static void ERROR(Args&&... args) {
        QUILL_LOG_ERROR(quill_logger, Fmt.array.data(), std::forward<Args>(args)...);
    }

    template <CtString Fmt, typename... Args>
    static void CRITICAL(Args&&... args) {
        QUILL_LOG_CRITICAL(quill_logger, Fmt.array.data(), std::forward<Args>(args)...);
    }

private:
    Logger();

    std::shared_ptr<quill::Handler> initFileHandler(const quill::fs::path& path,
                                                    quill::LogLevel level) const;
    std::shared_ptr<quill::Handler> initStdOutHandler(quill::LogLevel level) const;

    static std::atomic<bool> inited;
    inline static quill::Logger* quill_logger{nullptr};
};

enum LogLevel {
    L3,
    L2,
    L1,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

template <LogLevel Lvl, CtString Fmt, typename... Args>
void LOG(Args&&... args) {
    if constexpr (Lvl == L3) {
        Logger::L3<Fmt>(std::forward<Args>(args)...);
    } else if constexpr (Lvl == L2) {
        Logger::L2<Fmt>(std::forward<Args>(args)...);
    } else if constexpr (Lvl == L1) {
        Logger::L1<Fmt>(std::forward<Args>(args)...);
    } else if constexpr (Lvl == DEBUG) {
        Logger::DEBUG<Fmt>(std::forward<Args>(args)...);
    } else if constexpr (Lvl == INFO) {
        Logger::INFO<Fmt>(std::forward<Args>(args)...);
    } else if constexpr (Lvl == WARNING) {
        Logger::WARNING<Fmt>(std::forward<Args>(args)...);
    } else if constexpr (Lvl == ERROR) {
        Logger::ERROR<Fmt>(std::forward<Args>(args)...);
    } else if constexpr (Lvl == CRITICAL) {
        Logger::CRITICAL<Fmt>(std::forward<Args>(args)...);
    } else {
        []<bool B = false>() { static_assert(B, "Unknown Logger::Level"); };
    }
}

} // namespace gnev