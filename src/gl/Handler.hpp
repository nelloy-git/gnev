#pragma once

#include <memory>

#include "gl/Ctx.hpp"
#include "gl/fmt/HandlerLog.hpp"
#include "util/Export.hpp"
#include "util/SrcLoc.hpp"

namespace gnev::gl {

// class HandlerLog;

class EXPORT Handler {
public:
    Handler(GLuint* handle, void (*deleter)(GLuint*));
    Handler(const Handler&) = delete;
    Handler(Handler&&) = default;

    virtual ~Handler() = default;

    inline GLuint handle() const { return *_handle; };

protected:
    static constexpr CtString Msg{" => \"{}\""};

    template <std::size_t ArgsN>
    static constexpr CtString Args =
        CtStringConcat<CtString{"("},
                       CtStringRepeatSep<CtString{"{}"}, CtString{", "}, ArgsN>(),
                       CtString{")"}>();

    template <std::size_t ArgsN>
    static constexpr CtString ArgsRes = CtStringConcat<Args<ArgsN>, CtString{" -> {}"}>();

    std::unique_ptr<HandlerLog> Log(const SrcLoc src_loc = SrcLoc::Current()) const {
        return std::make_unique<HandlerLog>(src_loc, *_handle);
    };

private:
    std::unique_ptr<GLuint, void (*)(GLuint*)> _handle;
};

} // namespace gnev::gl