// #pragma once

// #include "Mat4x4.hpp"
// #include "gl/buffer/HeapBufferRange.hpp"
// #include "glm/glm.hpp"

// namespace gnev {

// class Transform {
//     friend class TransformStorage;

// public:
//     struct GL {
//         alignas(16) glm::vec3 position{1.f};
//         alignas(16) glm::vec3 euler{0.f};
//         alignas(16) glm::vec3 scale{1.f};

//         alignas(16) struct {
//             unsigned mat;
//             unsigned parent;
//         } index;
//     };

//     static constexpr unsigned int InvalidIndex = std::numeric_limits<unsigned>::max();
//     static constexpr GL Initial{.position{1.f},
//                                 .euler{0.f},
//                                 .scale{1.f},
//                                 .index = {.mat = InvalidIndex, .parent = InvalidIndex}};

//     Transform(const Mat4x4& mat, const gl::HeapBufferRange<GL>& range);
//     virtual ~Transform() = default;

//     unsigned getIndex() const;
//     const Mat4x4& getMat() const;

//     void setParent(const std::optional<Transform>& parent);
//     unsigned getParentIndex() const;
//     void setParentIndex(unsigned parent_index);

//     glm::vec3 getPosition() const;
//     void setPosition(const glm::vec3& position);

//     glm::vec3 getEuler() const;
//     void setEuler(const glm::vec3& euler);

//     glm::vec3 getScale() const;
//     void setScale(const glm::vec3& scale);

// private:
//     Mat4x4 mat;
//     gl::HeapBufferRange<GL> gl;

//     glm::vec3 position{1.f};
//     glm::vec3 euler{0.f};
//     glm::vec3 scale{1.f};

//     void applyMat();
// };

// } // namespace gnev