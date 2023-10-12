#pragma once

#include "functional"
#include "gl/Ctx.hpp"

namespace gnev::gl {

class EXPORT Handler {
public:
    Handler(GLuint* handle, void(*deleter)(GLuint*));
    Handler(const Handler&) = delete;
    Handler(Handler&&) = default;

    virtual ~Handler();

    inline GLuint handle() const { return *_handle; };

private:
    std::unique_ptr<GLuint, void(*)(GLuint*)> _handle;
};

} // namespace gnev::gl