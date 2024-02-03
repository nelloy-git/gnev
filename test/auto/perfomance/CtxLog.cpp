#include "gl/fmt/CtxLog.hpp"

#include <chrono>
#include <iostream>

#include "gtest/gtest.h"
#include "quill/Quill.h"

using namespace gnev;

class CtxLogPerfomace : public testing::Test {};

TEST_F(CtxLogPerfomace, info_log) {
    static constexpr std::size_t N = 100000; //0000;

    auto stdout_handler = quill::stdout_handler();
    stdout_handler->set_log_level(quill::LogLevel::Debug);
    static_cast<quill::ConsoleHandler*>(stdout_handler.get())->enable_console_colours();

    quill::Config cfg;
    cfg.default_handlers.push_back(stdout_handler);
    quill::configure(cfg);
    quill::start();
    
    auto* quill_logger = quill::create_logger("test", {stdout_handler});

    auto start_fmt = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < N; ++i){
        QUILL_LOG_INFO(quill_logger, "{}", fmtquill::format("{}, {}, {}", 0, "azaza", i));
    }
    auto end_fmt = std::chrono::high_resolution_clock::now();

    auto start_ctx = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < N; ++i){
        QUILL_LOG_INFO(quill_logger, "{}, {}, {}", 0, "azaza", i);
    }
    auto end_ctx = std::chrono::high_resolution_clock::now();

    QUILL_LOG_INFO(quill_logger, "std::format - {}", end_fmt - start_fmt);
    QUILL_LOG_INFO(quill_logger, "std::ctx - {}", end_ctx - start_ctx);
}