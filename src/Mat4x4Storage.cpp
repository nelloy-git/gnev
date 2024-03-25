#include "Mat4x4Storage.hpp"

#include "gl/buffer/BufferAccessorMappedCoherent.hpp"

namespace gnev {

Mat4x4Storage::Mat4x4Storage(std::unique_ptr<gl::ManagedBuffer>&& buffer)
    : buffer{std::move(buffer)} {}

std::optional<Mat4x4> Mat4x4Storage::create() {
    auto sub_buffer_opt = buffer->createSub<Mat4x4GL>();
    if (not sub_buffer_opt.has_value()) {
        return std::nullopt;
    }
    return Mat4x4{sub_buffer_opt.value()};
}

} // namespace gnev