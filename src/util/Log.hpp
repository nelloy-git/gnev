#pragma once

#include <atomic>
#include <memory>

#include "quill/Quill.h"
#include "util/Export.hpp"

namespace gnev {

static constexpr auto LOGGER_NAME = "gnev";

#define GNEV_TRACE_L3(fmt, ...) QUILL_LOG_TRACE_L3(quill::get_logger(LOGGER_NAME), fmt, ##__VA_ARGS__)
#define GNEV_TRACE_L2(fmt, ...) QUILL_LOG_TRACE_L2(quill::get_logger(LOGGER_NAME), fmt, ##__VA_ARGS__)
#define GNEV_DEBUG(fmt, ...) QUILL_LOG_DEBUG(quill::get_logger(LOGGER_NAME), fmt, ##__VA_ARGS__)
#define GNEV_INFO(fmt, ...) QUILL_LOG_INFO(quill::get_logger(LOGGER_NAME), fmt, ##__VA_ARGS__)
#define GNEV_CRITICAL(fmt, ...) QUILL_LOG_CRITICAL(quill::get_logger(LOGGER_NAME), fmt, ##__VA_ARGS__)

class EXPORT Log {
public:
    static void init();

private:
    Log();

    static std::atomic<bool> inited;
    static quill::Logger* quill_logger;
};
} // namespace gnev