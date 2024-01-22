#include "util/Log.hpp"

namespace gnev {

std::atomic<bool> Log::inited = false;
quill::Logger* Log::quill_logger = nullptr;

void Log::init() {
    bool is_already_inited = inited.exchange(true);
    if (is_already_inited) {
        return;
    }
    static Log log;
}

Log::Log() {
    auto file_handler = quill::file_handler("app.log", []() {
        quill::FileHandlerConfig cfg;
        cfg.set_open_mode('w');
        return cfg;
    }());
    file_handler->set_log_level(quill::LogLevel::TraceL3);
    file_handler->set_pattern("%(ascii_time) [%(thread)] %(logger_name) %(level_name) - "
                              "%(message)",              // format
                              "%H:%M:%S.%Qms",     // timestamp format
                              quill::Timezone::GmtTime); // timestamp's timezone

    auto stdout_handler = quill::stdout_handler();
    stdout_handler->set_log_level(quill::LogLevel::Debug);
    static_cast<quill::ConsoleHandler*>(stdout_handler.get())->enable_console_colours();

    quill::Config cfg;
    cfg.default_handlers.push_back(file_handler);
    cfg.default_handlers.push_back(stdout_handler);
    quill::configure(cfg);
    quill::start();

    Log::quill_logger = quill::create_logger(LOGGER_NAME, {file_handler, stdout_handler});
    Log::quill_logger->set_log_level(quill::LogLevel::TraceL3);

    // quill_logger->log()

    // quill_logger->set_log_level(quill::LogLevel::TraceL3);
}

} // namespace gnev