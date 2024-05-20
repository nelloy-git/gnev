#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <memory>

#include "gl/container/BufferPool.hpp"
#include "gl/container/BufferPoolElement.hpp"
#include "gl/container/BufferReflAccessor.hpp"

class Camera {
public:
    struct GL {
        static constexpr unsigned int InvalidIndex =
            std::numeric_limits<unsigned int>::max();

        struct MatIndexes {
            glm::uint view = InvalidIndex;
            glm::uint proj = InvalidIndex;
        };

        gnev::refl::Aligned<glm::vec3, 16> position = {{0, 0, 0}};
        gnev::refl::Aligned<glm::vec3, 16> direction = {{1, 0, 0}};
        gnev::refl::Aligned<glm::vec3, 16> top = {{0, 1, 0}};
        gnev::refl::Aligned<MatIndexes, 16> mats;
    };

    using Container = gnev::gl::BufferPool<GL>;
    using Accessor = gnev::gl::BufferReflAccessor<GL>;

    Camera(const std::shared_ptr<Container>& container, gnev::gl::BufferPoolElement<glm::mat4x4> view_mat,  const GL& initial);

    Accessor operator->();
    const Accessor operator->() const;

private:
    std::shared_ptr<Container> container;
    gnev::gl::Range range;
};