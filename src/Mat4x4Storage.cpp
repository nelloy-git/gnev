#include "Mat4x4Storage.hpp"

#include "gl/buffer/BufferAccessorMappedCoherent.hpp"

namespace gnev {

Mat4x4Storage::Mat4x4Storage(std::unique_ptr<gl::IBufferAccessor>&& accessor)
    : heap_buffer{std::move(accessor)} {}

Mat4x4Storage::Mat4x4Storage(unsigned capacity)
    : heap_buffer{initCoherentAccessor(capacity)} {}

std::optional<Mat4x4> Mat4x4Storage::create() {
    auto range_opt = heap_buffer.allocate(1);
    if (not range_opt.has_value()) {
        return std::nullopt;
    }
    return Mat4x4{range_opt.value()};
}

std::unique_ptr<gl::IBufferAccessor>
Mat4x4Storage::initCoherentAccessor(unsigned capacity) {
    auto buffer = std::make_unique<gl::Buffer>();
    buffer->initStorage(capacity * sizeof(Mat4x4::GL),
                        nullptr,
                        GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT |
                            GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    return std::make_unique<gl::BufferAccessorMappedCoherent>(std::move(buffer));
}

} // namespace gnev