#pragma once

#include <memory>

#include "gl/container/BufferReflManagedArray.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "util/Export.hpp"

namespace gnev {

class EXPORT Mat4x4Storage : public gl::BufferReflManagedArray<glm::mat4x4> {
public:
    Mat4x4Storage(std::unique_ptr<gl::IBufferRawAccessor>&& accessor, std::size_t fill_batch = 0);
    Mat4x4Storage(const Mat4x4Storage&) = delete;
    Mat4x4Storage(Mat4x4Storage&&) = default;
    virtual ~Mat4x4Storage() = default;
};

} // namespace gnev