#pragma once

#include <memory>

#include "Ctx.hpp"
#include "boost/preprocessor/control/if.hpp"
#include "boost/preprocessor/seq/enum.hpp"
#include "boost/preprocessor/seq/for_each.hpp"
#include "boost/preprocessor/stringize.hpp"
#include "boost/preprocessor/variadic/size.hpp"
#include "boost/preprocessor/variadic/to_seq.hpp"
#include "util/Export.hpp"
#include "util/SrcLoc.hpp"

namespace gnev::gl {

class HandlerLogger;

class EXPORT Handler {
public:
    Handler(GLuint* handle, void (*deleter)(GLuint*));
    Handler(const Handler&) = delete;
    Handler(Handler&&) = default;

    virtual ~Handler() = default;

    inline GLuint handle() const { return *_handle; };

private:
    std::unique_ptr<GLuint, void (*)(GLuint*)> _handle;
};

#define GNEV_HANDLER_LOG_L2(...)                                                         \
    GNEV_LOG_L2(BOOST_PP_IF(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__),                         \
                            GNEV_HANDLER_LOG_L2_NONEMPTY_VA_HELPER(                      \
                                BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)),                  \
                            "{}<{}>::{}()"),                                             \
                gnev::getTypeName<                                                       \
                    std::remove_const_t<std::remove_pointer_t<decltype(this)>>>(),       \
                handle(),                                                                \
                __FUNCTION__,                                                            \
                ##__VA_ARGS__)

#define GNEV_HANDLER_LOG_L1(...)                                                         \
    GNEV_LOG_L1(BOOST_PP_IF(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__),                         \
                            GNEV_HANDLER_LOG_L2_NONEMPTY_VA_HELPER(                      \
                                BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)),                  \
                            "{}<{}>::{}()"),                                             \
                gnev::getTypeName<                                                       \
                    std::remove_const_t<std::remove_pointer_t<decltype(this)>>>(),       \
                handle(),                                                                \
                __FUNCTION__,                                                            \
                ##__VA_ARGS__)

#define GNEV_HANDLER_LOG_L2_NONEMPTY_VA_HELPER(seq)                                      \
    "{}<{}>::{}" BOOST_PP_STRINGIZE(                                                     \
        (BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(GNEV_HANDLER_LOG_L2_NONEMPTY_VA_ELEM_HELPER, _, seq))))

#define GNEV_HANDLER_LOG_L2_NONEMPTY_VA_ELEM_HELPER(r, data, elem) ({})

#define GNEV_HANDLER_LOG_FMT(args_n)

} // namespace gnev::gl