#pragma once

#include <memory>

#include "Ctx.hpp"
#include "util/Export.hpp"
#include "util/Log.hpp"
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

#define GNEV_HANDLER_LOG(Level, ...)                                                     \
    BOOST_PP_CAT(GNEV_LOG_, Level)                                                       \
    ("{}#{}::{}" GNEV_ARGS_STR(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)),                     \
     GNEV_GET_TYPE_NAME(*this),                                                           \
     this->handle(),                                                                     \
     GNEV_GET_FUNC_NAME,                                                                 \
     ##__VA_ARGS__)

} // namespace gnev::gl