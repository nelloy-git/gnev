#pragma once

#include <memory>

#include "gl/Ctx.hpp"
#include "util/Export.hpp"
#include "util/SrcLoc.hpp"

namespace gnev::gl {

class HandlerLogger;

class EXPORT Handler {
public:
    Handler(GLuint* handle,
            void (*deleter)(GLuint*));
    Handler(const Handler&) = delete;
    Handler(Handler&&) = default;

    virtual ~Handler() = default;

    inline GLuint handle() const { return *_handle; };

protected:
    HandlerLogger Log(const SrcLoc& src_loc = SrcLoc::Current()) const;

private:
    std::unique_ptr<GLuint, void (*)(GLuint*)> _handle;
};

} // namespace gnev::gl