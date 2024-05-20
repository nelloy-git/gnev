#include "Ctx.hpp"
#include "boost/preprocessor.hpp"
#include "quill/detail/LogMacros.h"

#ifndef GNEV_GET_FUNC_NAME
#define GNEV_GET_FUNC_NAME __FUNCTION__
#endif

#ifndef GNEV_GET_TYPE_NAME

namespace gnev::details {


template <typename T>
constexpr auto getTypeName() {
    std::string_view name, prefix, suffix;
#ifdef __clang__
    name = __PRETTY_FUNCTION__;
    prefix = "auto gnev::details::getTypeName() [T = ";
    suffix = "]";
#elif defined(__GNUC__)
    name = __PRETTY_FUNCTION__;
    prefix = "constexpr auto gnev::details::getTypeName() [with T = ";
    suffix = "]";
#elif defined(_MSC_VER)
    name = __FUNCSIG__;
    prefix = "auto __cdecl gnev::details::getTypeName<";
    suffix = ">(void)";
#endif
    name.remove_prefix(prefix.size());
    name.remove_suffix(suffix.size());
    return name;
};

} // namespace gnev::details

#define GNEV_GET_TYPE_NAME(V) gnev::details::getTypeName<std::remove_cvref_t<decltype(V)>>()
#endif

#define GNEV_VA_SIZE(...)                                                                \
    BOOST_PP_IF(BOOST_PP_IS_EMPTY(__VA_ARGS__), 0, BOOST_PP_VARIADIC_SIZE(__VA_ARGS__))

#define GNEV_LOG_L3(fmt, ...)                                                            \
    do {                                                                                 \
        auto* quill_logger = gnev::Ctx::Get().getQuillLogger();                          \
        if (quill_logger) {                                                              \
            QUILL_LOG_TRACE_L3(quill_logger, fmt, ##__VA_ARGS__);                        \
        }                                                                                \
                                                                                         \
    } while (0)

#define GNEV_LOG_L2(fmt, ...)                                                            \
    do {                                                                                 \
        auto* quill_logger = gnev::Ctx::Get().getQuillLogger();                          \
        if (quill_logger) {                                                              \
            QUILL_LOG_TRACE_L2(quill_logger, fmt, ##__VA_ARGS__);                        \
        }                                                                                \
                                                                                         \
    } while (0)

#define GNEV_LOG_L1(fmt, ...)                                                            \
    do {                                                                                 \
        auto* quill_logger = gnev::Ctx::Get().getQuillLogger();                          \
        if (quill_logger) {                                                              \
            QUILL_LOG_TRACE_L1(quill_logger, fmt, ##__VA_ARGS__);                        \
        }                                                                                \
                                                                                         \
    } while (0)

#define GNEV_LOG_DEBUG(fmt, ...)                                                         \
    do {                                                                                 \
        auto* quill_logger = gnev::Ctx::Get().getQuillLogger();                          \
        if (quill_logger) {                                                              \
            QUILL_LOG_DEBUG(quill_logger, fmt, ##__VA_ARGS__);                           \
        }                                                                                \
                                                                                         \
    } while (0)

#define GNEV_LOG_INFO(fmt, ...)                                                          \
    do {                                                                                 \
        auto* quill_logger = gnev::Ctx::Get().getQuillLogger();                          \
        if (quill_logger) {                                                              \
            QUILL_LOG_INFO(quill_logger, fmt, ##__VA_ARGS__);                            \
        }                                                                                \
                                                                                         \
    } while (0)

#define GNEV_LOG_WARNING(fmt, ...)                                                       \
    do {                                                                                 \
        auto* quill_logger = gnev::Ctx::Get().getQuillLogger();                          \
        if (quill_logger) {                                                              \
            QUILL_LOG_WARNING(quill_logger, fmt, ##__VA_ARGS__);                         \
        }                                                                                \
                                                                                         \
    } while (0)

#define GNEV_LOG_ERROR(fmt, ...)                                                         \
    do {                                                                                 \
        auto* quill_logger = gnev::Ctx::Get().getQuillLogger();                          \
        if (quill_logger) {                                                              \
            QUILL_LOG_ERROR(quill_logger, fmt, ##__VA_ARGS__);                           \
        }                                                                                \
                                                                                         \
    } while (0)

#define GNEV_LOG_CRITICAL(fmt, ...)                                                      \
    do {                                                                                 \
        auto* quill_logger = gnev::Ctx::Get().getQuillLogger();                          \
        if (quill_logger) {                                                              \
            QUILL_LOG_CRITICAL(quill_logger, fmt, ##__VA_ARGS__);                        \
        }                                                                                \
                                                                                         \
    } while (0)

#define GNEV_LOG(level, fmt, ...) BOOST_PP_CAT(GNEV_LOG_, level)(fmt, ##__VA_ARGS__)

#define GNEV_REPEAT_STR(str, sep, count)                                                 \
    BOOST_PP_IF(count,                                                                   \
                BOOST_PP_REPEAT(BOOST_PP_SUB(count, 1),                                  \
                                GNEV_REPEAT_STR_HELPER,                                  \
                                (str)(sep)) str,                                         \
                BOOST_PP_EMPTY())

#define GNEV_REPEAT_STR_HELPER(z, n, data)                                               \
    BOOST_PP_SEQ_ELEM(0, data) BOOST_PP_SEQ_ELEM(1, data)

#define GNEV_ARGS_STR(count) "(" GNEV_REPEAT_STR("{}", ", ", count) ")"