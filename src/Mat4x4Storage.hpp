#pragma once

#include "gl/ReflStruct.hpp"
#include "gl/buffer/ManagedBuffer.hpp"
#include "glm/mat4x4.hpp"

namespace gnev {

GNEV_REFL_STRUCT_DECLARE(Mat4x4GL, ((alignas(64))glm::mat4x4)mat);

class Mat4x4 : public gl::ManagedSubBuffer<Mat4x4GL> {
public:
    Mat4x4(const gl::ManagedSubBuffer<Mat4x4GL>& parent)
        : gl::ManagedSubBuffer<Mat4x4GL>{parent} {}

    std::size_t getIndex() const { return getOffset() / sizeof(Mat4x4GL); }
};

class EXPORT Mat4x4Storage {
public:
    Mat4x4Storage(std::unique_ptr<gl::ManagedBuffer>&& buffer);
    virtual ~Mat4x4Storage() = default;

    std::optional<Mat4x4> create();

private:
    std::unique_ptr<gl::ManagedBuffer>&& buffer;
};
} // namespace gnev