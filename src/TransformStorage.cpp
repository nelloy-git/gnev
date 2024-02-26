#include "TransformStorage.hpp"

#include "gl/buffer/BufferAccessorMappedCoherent.hpp"

namespace gnev {

TransformStorage::TransformStorage(std::unique_ptr<gl::IBufferAccessor>&& accessor,
                                   const std::shared_ptr<Mat4x4Storage>& mat_storage)
    : mat_storage{mat_storage}
    , heap_buffer{std::move(accessor)} {
    heap_buffer.setMoveCallback([](gl::IBufferAccessor& accessor,
                                   gl::BufferRangePos src,
                                   gl::BufferRangePos dst) {
        accessor.copy(src.offset, dst.offset, dst.size);
    });
    heap_buffer.setFreeCallback([](gl::IBufferAccessor& accessor,
                                   gl::BufferRangePos pos) {
        accessor.set(pos.offset, pos.size, &Transform::Initial);
    });
};

TransformStorage::TransformStorage(unsigned capacity,
                                   const std::shared_ptr<Mat4x4Storage>& mat_storage)
    : mat_storage{mat_storage}
    , heap_buffer{initCoherentAccessor(capacity)} {}

std::optional<Transform> TransformStorage::create() {
    auto mat_opt = mat_storage->create();
    if (not mat_opt.has_value()) {
        return std::nullopt;
    }
    auto range_opt = heap_buffer.allocate(1);
    if (not range_opt.has_value()) {
        return std::nullopt;
    }
    return Transform{mat_opt.value(), range_opt.value()};
}

std::unique_ptr<gl::IBufferAccessor>
TransformStorage::initCoherentAccessor(unsigned capacity) {
    auto buffer = std::make_unique<gl::Buffer>();
    buffer->initStorage(capacity * sizeof(Transform::GL),
                        nullptr,
                        GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT |
                            GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    return std::make_unique<gl::BufferAccessorMappedCoherent>(std::move(buffer));
}

} // namespace gnev