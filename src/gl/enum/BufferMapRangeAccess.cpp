#include "gl/enum/BufferMapRangeAccess.hpp"

#include "magic_enum/magic_enum_flags.hpp"

namespace gnev::gl {

std::string format_as(BufferMapRangeAccess value) {
    return magic_enum::enum_flags_name(value);
}

} // namespace gnev::gl