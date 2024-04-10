#pragma once

#include <atomic>
#include <memory>

#include "quill/Quill.h"
#include "util/CtString.hpp"
#include "util/Export.hpp"

namespace gnev {

enum class LogLevel {
    L3,
    L2,
    L1,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger final {
public:
    Logger();
    ~Logger() = default;

    void setQuillLogger(quill::Logger* quill_logger);
    quill::Logger* getQuillLogger();

    template <LogLevel Level, CtString Fmt, typename... Args>
    inline void log(Args&&... args) const {
        using enum LogLevel;
        if constexpr (Level == L3) {
            QUILL_LOG_TRACE_L3(quill_logger.load(),
                               Fmt.array.data(),
                               std::forward<Args>(args)...);
        } else if constexpr (Level == L2) {
            QUILL_LOG_TRACE_L2(quill_logger.load(),
                               Fmt.array.data(),
                               std::forward<Args>(args)...);
        } else if constexpr (Level == L1) {
            QUILL_LOG_TRACE_L1(quill_logger.load(),
                               Fmt.array.data(),
                               std::forward<Args>(args)...);
        } else if constexpr (Level == DEBUG) {
            QUILL_LOG_DEBUG(quill_logger.load(),
                            Fmt.array.data(),
                            std::forward<Args>(args)...);
        } else if constexpr (Level == INFO) {
            QUILL_LOG_INFO(quill_logger.load(),
                           Fmt.array.data(),
                           std::forward<Args>(args)...);
        } else if constexpr (Level == WARNING) {
            QUILL_LOG_WARNING(quill_logger.load(),
                              Fmt.array.data(),
                              std::forward<Args>(args)...);
        } else if constexpr (Level == ERROR) {
            QUILL_LOG_ERROR(quill_logger.load(),
                            Fmt.array.data(),
                            std::forward<Args>(args)...);
        } else if constexpr (Level == CRITICAL) {
            QUILL_LOG_CRITICAL(quill_logger.load(),
                               Fmt.array.data(),
                               std::forward<Args>(args)...);
        }
    }

private:
    std::atomic<quill::Logger*> quill_logger;
};

} // namespace gnev