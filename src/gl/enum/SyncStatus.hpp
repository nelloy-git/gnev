#pragma once

#include <string_view>

#include "Ctx.hpp"

namespace gnev::gl {

enum class SyncStatus : GLenum {
    ALREADY_SIGNALED = GL_ALREADY_SIGNALED,
    TIMEOUT_EXPIRED = GL_TIMEOUT_EXPIRED,
    CONDITION_SATISFIED = GL_CONDITION_SATISFIED,
    WAIT_FAILED = GL_WAIT_FAILED
};

std::string_view format_as(SyncStatus val);

} // namespace gnev::gl