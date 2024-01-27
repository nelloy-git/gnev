#pragma once

#include <source_location>

#include "gl/Ctx.hpp"
#include "util/CtString.hpp"

namespace gnev::gl {

class HandlerLog;

class EXPORT Handler {
public:
    Handler(GLuint* handle, void (*deleter)(GLuint*));
    Handler(const Handler&) = delete;
    Handler(Handler&&) = default;

    virtual ~Handler() = default;

    inline GLuint handle() const { return *_handle; };

protected:
    std::unique_ptr<HandlerLog>
    L2(const CtString<>& class_name = getClassName(),
       const CtString<>& method_name = getMethodName()) const;

private:
    std::unique_ptr<GLuint, void (*)(GLuint*)> _handle;
};

} // namespace gnev::gl