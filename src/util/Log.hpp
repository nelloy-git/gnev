#pragma once

#include <atomic>
#include <memory>
#include <source_location>

#include "quill/Quill.h"
#include "util/CtString.hpp"
#include "util/Export.hpp"

namespace gnev {

class EXPORT Log {
public:
    static constexpr auto LOGGER_NAME = "gnev";

    static void init();

    template <auto Fmt = CtString{"{}"}>
    inline static void L3(auto&&... args) {
        QUILL_LOG_TRACE_L3(quill_logger, Fmt.array.data(), args...);
    }

    template <auto Fmt = CtString{"{}"}>
    inline static void L2(auto&&... args) {
        QUILL_LOG_TRACE_L2(quill_logger, Fmt.array.data(), args...);
    }

    template <auto Fmt = CtString{"{}"}>
    inline static void L1(auto&&... args) {
        QUILL_LOG_TRACE_L3(quill_logger, Fmt.array.data(), args...);
    }

    template <auto Fmt = CtString{"{}"}>
    inline static void DEBUG(auto&&... args) {
        QUILL_LOG_DEBUG(quill_logger, Fmt.array.data(), args...);
    }

    template <auto Fmt = CtString{"{}"}>
    inline static void INFO(auto&&... args) {
        QUILL_LOG_INFO(quill_logger, Fmt.array.data(), args...);
    }

    template <auto Fmt = CtString{"{}"}>
    inline static void ERROR(auto&&... args) {
        QUILL_LOG_ERROR(quill_logger, Fmt.array.data(), args...);
    }

    template <auto Fmt = CtString{"{}"}>
    inline static void CRITICAL(auto&&... args) {
        QUILL_LOG_CRITICAL(quill_logger, Fmt.array.data(), args...);
    }

private:
    Log();

    static std::atomic<bool> inited;
    inline static quill::Logger* quill_logger = nullptr;
};

} // namespace gnev