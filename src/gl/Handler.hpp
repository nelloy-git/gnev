#pragma once

#include <source_location>

#include "gl/Ctx.hpp"
#include "util/CtString.hpp"

namespace gnev::gl {

class HandlerTraceL2;

class EXPORT Handler {
public:
    Handler(GLuint* handle, void (*deleter)(GLuint*));
    Handler(const Handler&) = delete;
    Handler(Handler&&) = default;

    virtual ~Handler() = default;

    inline GLuint handle() const { return *_handle; };

protected:
    std::unique_ptr<HandlerTraceL2>
    L2(const CtString<128>& class_name = getClassName(),
       const CtString<128>& method_name = getMethodName()) const;

    // GNEV_REGISTER_HANDLER_TRACE_L2(TRACE_L2, WITH_RES_L2);

private:
    std::unique_ptr<GLuint, void (*)(GLuint*)> _handle;
};

} // namespace gnev::gl