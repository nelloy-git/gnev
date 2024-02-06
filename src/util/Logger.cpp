#include "util/Logger.hpp"

namespace gnev {

std::atomic<bool> Logger::inited = false;

void Logger::init() {
    bool is_already_inited = inited.exchange(true);
    if (is_already_inited) {
        return;
    }
    static Logger log;
}

Logger::Logger() {
    auto file_full_handler = initFileHandler("full.log", quill::LogLevel::TraceL3);
    auto file_short_handler = initFileHandler("short.log", quill::LogLevel::Debug);
    auto stdout_handler = initStdOutHandler(quill::LogLevel::Debug);

    quill::Config cfg;
    cfg.default_handlers.push_back(file_full_handler);
    cfg.default_handlers.push_back(file_short_handler);
    cfg.default_handlers.push_back(stdout_handler);
    quill::configure(cfg);
    quill::start();

    Logger::quill_logger =
        quill::create_logger(LOGGER_NAME,
                             {file_full_handler, file_short_handler, stdout_handler});
    Logger::quill_logger->set_log_level(quill::LogLevel::TraceL3);

    Logger::quill_logger->init_backtrace(10);
}

std::shared_ptr<quill::Handler> Logger::initFileHandler(const quill::fs::path& path,
                                                        quill::LogLevel level) const {
    auto handler = quill::file_handler(path, []() {
        quill::FileHandlerConfig cfg;
        cfg.set_open_mode('w');
        return cfg;
    }());
    handler->set_pattern("%(ascii_time) [%(thread)] %(logger_name:<9) %(level_name:<12)"
                         "%(message)",              // format
                         "%H:%M:%S.%Qms",           // timestamp format
                         quill::Timezone::GmtTime); // timestamp's timezone
    handler->set_log_level(level);
    return handler;
}

std::shared_ptr<quill::Handler> Logger::initStdOutHandler(quill::LogLevel level) const {
    auto handler = quill::stdout_handler();
    handler->set_log_level(quill::LogLevel::Debug);
    static_cast<quill::ConsoleHandler*>(handler.get())->enable_console_colours();
    return handler;
}

} // namespace gnev