#include "gl/SyncFence.hpp"

#include <optional>

#include "Ctx.hpp"
#include "gl/enum/SyncStatus.hpp"
#include "util/Log.hpp"

#define GNEV_SYNC_FENCE_LOG(Level, ...)                                                  \
    GNEV_LOG(Level,                                                                      \
             "{}::{}" GNEV_ARGS_STR(GNEV_VA_SIZE(__VA_ARGS__)),                          \
             GNEV_GET_TYPE_NAME(*this),                                                  \
             GNEV_GET_FUNC_NAME,                                                         \
             ##__VA_ARGS__)

namespace gnev::gl {

SyncFence::SyncFence()
    : sync(Ctx::Get().glFenceSync()) {
    GNEV_SYNC_FENCE_LOG(L2);
}

SyncStatus SyncFence::wait(GLuint64 wait_ns) {
    GNEV_SYNC_FENCE_LOG(L2, wait_ns);
    last_status =
        static_cast<SyncStatus>(Ctx::Get().glClientWaitSync(sync,
                                                            GL_SYNC_FLUSH_COMMANDS_BIT,
                                                            wait_ns));
    GNEV_LOG(L2, "\treturn {}", *last_status);
    return *last_status;
}

std::optional<SyncStatus> SyncFence::lastStatus() const { return last_status; }

} // namespace gnev::gl

#undef GNEV_SYNC_FENCE_LOG