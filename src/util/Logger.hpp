#pragma once

#include <atomic>
#include <memory>

#include "quill/Quill.h"
#include "util/CtString.hpp"
#include "util/Export.hpp"

namespace gnev {

class Logger final {
public:
    enum class Level {
        L3,
        L2,
        L1,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

    Logger();
    ~Logger() = default;

    void setQuillLogger(quill::Logger* quill_logger);
    quill::Logger* getQuillLogger();

    template <CtString Fmt, typename... Args>
    inline void L3(Args&&... args) const {
        QUILL_LOG_TRACE_L3(quill_logger.load(), Fmt.array.data(), std::forward<Args>(args)...);
    }

    template <CtString Fmt, typename... Args>
    inline void L2(Args&&... args) const {
        QUILL_LOG_TRACE_L2(quill_logger.load(), Fmt.array.data(), std::forward<Args>(args)...);
    }

    template <CtString Fmt, typename... Args>
    inline void L1(Args&&... args) const {
        QUILL_LOG_TRACE_L1(quill_logger.load(), Fmt.array.data(), std::forward<Args>(args)...);
    }

    template <CtString Fmt, typename... Args>
    inline void DEBUG(Args&&... args) const {
        QUILL_LOG_DEBUG(quill_logger.load(), Fmt.array.data(), std::forward<Args>(args)...);
    }

    template <CtString Fmt, typename... Args>
    inline void INFO(Args&&... args) const {
        QUILL_LOG_INFO(quill_logger.load(), Fmt.array.data(), std::forward<Args>(args)...);
    }

    template <CtString Fmt, typename... Args>
    inline void WARNING(Args&&... args) const {
        QUILL_LOG_WARNING(quill_logger.load(), Fmt.array.data(), std::forward<Args>(args)...);
    }

    template <CtString Fmt, typename... Args>
    inline void ERROR(Args&&... args) const {
        QUILL_LOG_ERROR(quill_logger.load(), Fmt.array.data(), std::forward<Args>(args)...);
    }

    template <CtString Fmt, typename... Args>
    inline void CRITICAL(Args&&... args) const {
        QUILL_LOG_CRITICAL(quill_logger.load(), Fmt.array.data(), std::forward<Args>(args)...);
    }

    template<Level lvl, CtString Fmt, typename... Args>
    inline void log(Args&&... args) const {
        if constexpr (lvl == L3){
            L3<Fmt>(std::forward<Args>(args)...);
        } else if constexpr (lvl == L2){
            L2<Fmt>(std::forward<Args>(args)...);
        } else if constexpr (lvl == L1){
            L1<Fmt>(std::forward<Args>(args)...);
        } else if constexpr (lvl == DEBUG){
            DEBUG<Fmt>(std::forward<Args>(args)...);
        } else if constexpr (lvl == INFO){
            INFO<Fmt>(std::forward<Args>(args)...);
        } else if constexpr (lvl == WARNING){
            WARNING<Fmt>(std::forward<Args>(args)...);
        } else if constexpr (lvl == ERROR){
            ERROR<Fmt>(std::forward<Args>(args)...);
        } else if constexpr (lvl == CRITICAL){
            CRITICAL<Fmt>(std::forward<Args>(args)...);
        }
    }

private:
    std::atomic<quill::Logger*> quill_logger;
};

} // namespace gnev