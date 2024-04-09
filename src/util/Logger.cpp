#include "util/Logger.hpp"

namespace gnev {

Logger::Logger() { quill::get_logger()->set_log_level(quill::LogLevel::TraceL3); }

void Logger::setQuillLogger(quill::Logger* quill_logger_) {
    quill_logger = quill_logger_;
}

quill::Logger* Logger::getQuillLogger() { return quill_logger; }


} // namespace gnev