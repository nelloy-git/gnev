#pragma once

#include <memory>

#include "gl/container/BufferPool.hpp"
#include "gl/container/BufferReflAccessor.hpp"
#include "glm/ext/matrix_float4x4.hpp"

class Mat4x4 {
public:
    using GL = glm::mat4x4;

    using Container = gnev::gl::BufferPool<GL>;
    using Accessor = gnev::gl::BufferReflAccessor<GL>;

    Mat4x4(const std::shared_ptr<Container>& container,
           const GL& initial = GL{1.f});
    ~Mat4x4();

    unsigned index() const;
    gnev::gl::BufferReflAccessor<GL> operator->();
    const gnev::gl::BufferReflAccessor<GL> operator->() const;

private:
    std::weak_ptr<Container> weak_container;
    unsigned container_index;
};