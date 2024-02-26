#pragma once

#include "gl/buffer/HeapBufferRange.hpp"
#include "glm/glm.hpp"
#include "Mat4x4.hpp"

namespace gnev {

class EXPORT Mat4x4Storage {
public:
    Mat4x4Storage(std::unique_ptr<gl::IBufferAccessor>&& accessor);
    // Coherent storage
    Mat4x4Storage(unsigned capacity);
    virtual ~Mat4x4Storage() = default;

    std::optional<Mat4x4> create();

private:
    gl::HeapBuffer<Mat4x4::GL> heap_buffer;

    static std::unique_ptr<gl::IBufferAccessor> initCoherentAccessor(unsigned capacity);
};
} // namespace gnev