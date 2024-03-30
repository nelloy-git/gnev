#include "gl/enum/BufferMapRangeAccess.hpp"
#include "gl/enum/BufferStorageFlags.hpp"
#include "gl/enum/ShaderType.hpp"
#include "gl/enum/TextureFormat.hpp"
#include "gl/enum/TextureInternalFormat.hpp"
#include "gl/enum/TextureMagFilter.hpp"
#include "gl/enum/TextureMinFilter.hpp"
#include "gl/enum/TextureTarget.hpp"
#include "gl/enum/TextureType.hpp"
#include "gl/enum/TextureWrapS.hpp"
#include "gl/enum/TextureWrapT.hpp"
#include "magic_enum/magic_enum.hpp"

namespace gnev {

std::string_view format_as(gl::BufferMapRangeAccess value) {
    return magic_enum::enum_name(value);
}

std::string_view format_as(gl::BufferStorageFlags value) {
    return magic_enum::enum_name(value);
}

std::string_view format_as(ShaderType value) { return magic_enum::enum_name(value); }

std::string_view format_as(TextureFormat value) { return magic_enum::enum_name(value); }

std::string_view format_as(TextureInternalFormat value) {
    return magic_enum::enum_name(value);
}

std::string_view format_as(TextureMagFilter value) {
    return magic_enum::enum_name(value);
}

std::string_view format_as(TextureMinFilter value) {
    return magic_enum::enum_name(value);
}

std::string_view format_as(TextureTarget value) { return magic_enum::enum_name(value); }

std::string_view format_as(TextureType value) { return magic_enum::enum_name(value); }

std::string_view format_as(TextureWrapS value) { return magic_enum::enum_name(value); }

std::string_view format_as(TextureWrapT value) { return magic_enum::enum_name(value); }

} // namespace gnev