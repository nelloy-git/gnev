#pragma once

#include <type_traits>

#include "gl/data/VertexAttributeInfo.hpp"
#include "glm/detail/qualifier.hpp"

namespace gnev::gl {

namespace details {

template <AttributeType T>
struct DeduceUnpackedElement;

template <>
struct DeduceUnpackedElement<AttributeType::BYTE> {
    using Type = GLbyte;
};

template <>
struct DeduceUnpackedElement<AttributeType::UNSIGNED_BYTE> {
    using Type = GLubyte;
};

template <>
struct DeduceUnpackedElement<AttributeType::SHORT> {
    using Type = GLshort;
};

template <>
struct DeduceUnpackedElement<AttributeType::UNSIGNED_SHORT> {
    using Type = GLushort;
};

template <>
struct DeduceUnpackedElement<AttributeType::INT> {
    using Type = GLint;
};

template <>
struct DeduceUnpackedElement<AttributeType::UNSIGNED_INT> {
    using Type = GLuint;
};

template <>
struct DeduceUnpackedElement<AttributeType::FIXED> {
    using Type = GLfixed;
};

template <>
struct DeduceUnpackedElement<AttributeType::FLOAT> {
    using Type = GLfloat;
};

template <>
struct DeduceUnpackedElement<AttributeType::HALF_FLOAT> {
    using Type = GLhalf;
};

template <>
struct DeduceUnpackedElement<AttributeType::DOUBLE> {
    using Type = GLdouble;
};

template <AttributeType T, std::size_t S>
    requires(S > 0 and S <= 4)
struct DeduceUnpackedAttributeType {
    using Type = glm::vec<S, typename DeduceUnpackedElement<T>::Type>;
};

template <AttributeType T>
struct DeducePackedAttributeType {
    using Type = GLuint;
};

template <VertexAttributeInfo Info>
using DeduceAttributeType = std::conditional_t<
    Info.is_packed,
    typename DeducePackedAttributeType<Info.type>::Type,
    typename DeduceUnpackedAttributeType<Info.type, Info.elements>::Type>;

} // namespace details

template <VertexAttributeInfo Info>
using VertexAttribute = details::DeduceAttributeType<Info>;

// GNEV_TEMPLATE_CONCEPT(VertexAttribute, VertexAttributeInfo);

} // namespace gnev::gl