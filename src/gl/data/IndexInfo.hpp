#pragma once

#include <array>
#include <cstddef>

#include "glad/gl.h"

namespace gnev::gl {

enum class IndexType {
    UBYTE = GL_UNSIGNED_BYTE,
    USHORT = GL_UNSIGNED_SHORT,
    UINT = GL_UNSIGNED_INT,
};

namespace details {

template <IndexType T, std::size_t N>
struct DeduceIndexType;

template <std::size_t N>
struct DeduceIndexType<IndexType::UBYTE, N> {
    using Element = GLubyte;
    using Type = std::array<GLubyte, N>;
};

template <std::size_t N>
struct DeduceIndexType<IndexType::USHORT, N> {
    using Element = GLushort;
    using Type = std::array<GLushort, N>;
};

template <std::size_t N>
struct DeduceIndexType<IndexType::UINT, N> {
    using Element = GLuint;
    using Type = std::array<GLuint, N>;
};

} // namespace details


template <IndexType T, std::size_t N>
using IndexBlock = details::DeduceIndexType<T, N>::Type;

} // namespace gnev::gl