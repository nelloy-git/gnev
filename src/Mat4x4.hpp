// #pragma once

// #include "gl/buffer/HeapBufferRange.hpp"
// #include "glm/glm.hpp"

// namespace gnev {

// class Mat4x4 {
//     friend class Mat4x4Storage;

// public:
//     using GL = glm::mat4x4;
//     static constexpr GL Initial{1.f};

//     Mat4x4(const gl::HeapBufferRange<GL>& range);
//     virtual ~Mat4x4() = default;

//     unsigned getIndex() const;
//     glm::mat4x4 get() const;
//     void set(const glm::mat4x4& mat);

// private:
//     gl::HeapBufferRange<GL> gl;
// };

// } // namespace gnev