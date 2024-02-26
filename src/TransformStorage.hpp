#pragma once

#include "Mat4x4Storage.hpp"
#include "Transform.hpp"
#include "gl/buffer/HeapBufferRange.hpp"
#include "glm/glm.hpp"

namespace gnev {

class EXPORT TransformStorage {
public:
    TransformStorage(std::unique_ptr<gl::IBufferAccessor>&& accessor,
                     const std::shared_ptr<Mat4x4Storage>& mat_storage);
    // Coherent storage
    TransformStorage(unsigned capacity,
                     const std::shared_ptr<Mat4x4Storage>& mat_storage);
    virtual ~TransformStorage() = default;

    std::optional<Transform> create();

private:
    std::shared_ptr<Mat4x4Storage> mat_storage;
    gl::HeapBuffer<Transform::GL> heap_buffer;

    static std::unique_ptr<gl::IBufferAccessor> initCoherentAccessor(unsigned capacity);
};
} // namespace gnev