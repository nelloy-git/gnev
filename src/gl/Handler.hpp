#pragma once

#include "functional"
#include "gl/Ctx.hpp"

namespace gnev::gl {

class EXPORT Handler {
public:
    using FreeHandle = std::function<void(const Ctx&, GLuint)>;

    Handler(const Ctx& ctx, GLuint handle, const FreeHandle& deleter);
    Handler(const Handler&) = delete;
    Handler(Handler&&) = default;

    virtual ~Handler();

    inline const Ctx& ctx() const { return _ctx; };

    inline GLuint handle() const { return *_handle; };

private:
    Ctx _ctx;
    std::unique_ptr<GLuint> _handle;
    std::function<void(const Ctx&, GLuint)> _deleter;
};

} // namespace gnev::gl