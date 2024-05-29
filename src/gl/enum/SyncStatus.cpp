#include "gl/enum/SyncStatus.hpp"
#include <string_view>

namespace gnev::gl {

std::string_view format_as(SyncStatus val){
    switch (val) {
        using enum SyncStatus;
        case ALREADY_SIGNALED: return "ALREADY_SIGNALED";
        case TIMEOUT_EXPIRED: return "TIMEOUT_EXPIRED";
        case CONDITION_SATISFIED: return "CONDITION_SATISFIED";
        case WAIT_FAILED: return "WAIT_FAILED";
        default: return "UNKNOWN";
    }
}

}