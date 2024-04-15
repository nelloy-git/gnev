// #pragma once

// #include <memory>

// #include "gl/container/BufferReflAccessor.hpp"
// #include "gl/container/BufferReflArray.hpp"
// #include "glm/mat4x4.hpp"
// #include "util/Export.hpp"
// #include "util/IndexManager.hpp"
// #include "util/refl/Struct.hpp"

// namespace gnev {

// GNEV_REFL_STRUCT(Mat4x4_Refl, (alignas(16))(glm::mat4x4)(mat));

// class EXPORT Mat4x4Storage {
// public:
//     Mat4x4Storage(std::unique_ptr<gl::BufferReflArray<GlmMat4x4Meta>>&& array);
//     virtual ~Mat4x4Storage() = default;

//     std::optional<unsigned> reserveIndex();
//     bool freeIndex(unsigned index);
//     bool isInUse(unsigned index) const;

//     std::shared_ptr<unsigned> makeIndexGuard();

//     gl::BufferReflAccessor<GlmMat4x4Meta> operator[](unsigned i);
//     const gl::BufferReflAccessor<GlmMat4x4Meta> operator[](unsigned i) const;
//     gl::BufferReflAccessor<GlmMat4x4Meta> at(unsigned i);
//     const gl::BufferReflAccessor<GlmMat4x4Meta> at(unsigned i) const;

// private:
//     std::shared_ptr<IndexManager> manager;
//     std::unique_ptr<gl::BufferReflArray<GlmMat4x4Meta>> array;
// };

// } // namespace gnev