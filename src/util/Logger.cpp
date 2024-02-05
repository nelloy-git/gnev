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
    auto file_handler = quill::file_handler("app.log", []() {
        quill::FileHandlerConfig cfg;
        cfg.set_open_mode('w');
        return cfg;
    }());
    file_handler
        ->set_pattern("%(ascii_time) [%(thread)] %(logger_name:<9) %(level_name:<12)"
                      "%(message)",              // format
                      "%H:%M:%S.%Qms",           // timestamp format
                      quill::Timezone::GmtTime); // timestamp's timezone

    auto stdout_handler = quill::stdout_handler();
    stdout_handler->set_log_level(quill::LogLevel::Debug);
    static_cast<quill::ConsoleHandler*>(stdout_handler.get())->enable_console_colours();

    quill::Config cfg;
    cfg.default_handlers.push_back(file_handler);
    cfg.default_handlers.push_back(stdout_handler);
    quill::configure(cfg);
    quill::start();

    Logger::quill_logger =
        quill::create_logger(LOGGER_NAME, {file_handler, stdout_handler});
    Logger::quill_logger->set_log_level(quill::LogLevel::TraceL2);
}

} // namespace gnev