#pragma once

#include <atomic>
#include <memory>
#include <source_location>

#include "quill/Quill.h"
#include "util/Export.hpp"

namespace gnev {

static constexpr auto LOGGER_NAME = "gnev";

#define GNEV_TRACE_L3(fmt, ...)                                                          \
    QUILL_LOG_TRACE_L3(quill::get_logger(LOGGER_NAME), fmt, ##__VA_ARGS__)
#define GNEV_TRACE_L2(fmt, ...)                                                          \
    QUILL_LOG_TRACE_L2(quill::get_logger(LOGGER_NAME), fmt, ##__VA_ARGS__)
#define GNEV_DEBUG(fmt, ...)                                                             \
    QUILL_LOG_DEBUG(quill::get_logger(LOGGER_NAME), fmt, ##__VA_ARGS__)
#define GNEV_INFO(fmt, ...)                                                              \
    QUILL_LOG_INFO(quill::get_logger(LOGGER_NAME), fmt, ##__VA_ARGS__)
#define GNEV_ERROR(fmt, ...)                                                             \
    QUILL_LOG_ERROR(quill::get_logger(LOGGER_NAME), fmt, ##__VA_ARGS__)
#define GNEV_CRITICAL(fmt, ...)                                                          \
    QUILL_LOG_CRITICAL(quill::get_logger(LOGGER_NAME), fmt, ##__VA_ARGS__)

class EXPORT Log {
public:
    static void init();

private:
    Log();

    static std::atomic<bool> inited;
    static quill::Logger* quill_logger;
};

template <typename T>
constexpr std::string_view getClassName() {
#ifdef __clang__
    std::string_view str = __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
    std::string_view str = __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
    std::string_view str = __FUNCSIG__;
#else
#error "Unsupported compiler"
#endif
    auto start = str.find_last_of('<');
    auto end = str.find_last_of('>');

    return str.substr(start + 1, end - start);
}

} // namespace gnev