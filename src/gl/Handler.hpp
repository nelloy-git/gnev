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
    L2(const std::string_view& class_name = getClassName().to_string_view(),
       const std::string_view& method_name = getMethodName().to_string_view()) const;

private:
    std::unique_ptr<GLuint, void (*)(GLuint*)> _handle;
};

} // namespace gnev::gl