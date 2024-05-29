#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>
#include <utility>

#include "boost/preprocessor/punctuation/remove_parens.hpp"
#include "boost/preprocessor/seq/elem.hpp"
#include "boost/preprocessor/seq/for_each.hpp"
#include "boost/preprocessor/seq/for_each_i.hpp"
#include "boost/preprocessor/seq/to_tuple.hpp"
#include "gl/data/VertexAttributeInfo.hpp"
#include "gl/enum/AttributeType.hpp"
#include "util/Reflection.hpp"

namespace gnev::gl {

#define GNEV_VERTEX_GET_ATTR_INFO_NAME getAttrInfo
#define GNEV_VERTEX_GET_ATTR_OFFSET_NAME getAttrOffset

#define GNEV_VERTEX_ATTR(r, name, i, elem)                                               \
    gnev::gl::VertexAttribute<BOOST_PP_SEQ_ELEM(0, elem)> BOOST_PP_SEQ_ELEM(1, elem);

#define GNEV_VERTEX_GET_ATTR_INFO(name, seq)                                             \
    template <gnev::refl::Key K>                                                         \
    static consteval const gnev::gl::VertexAttributeInfo&                                \
    GNEV_VERTEX_GET_ATTR_INFO_NAME() {                                                   \
        static constexpr GNEV_VERTEX_ATTR_INFO(seq);                                     \
        return Info[K.template index<name>()];                                           \
    }

#define GNEV_VERTEX_ATTR_INFO(seq)                                                       \
    std::array<gnev::gl::VertexAttributeInfo, BOOST_PP_SEQ_SIZE(seq)> Info = {           \
        BOOST_PP_REMOVE_PARENS(BOOST_PP_SEQ_TO_TUPLE(                                    \
            BOOST_PP_SEQ_FOR_EACH(GNEV_VERTEX_ATTR_INFO_LIST, _, seq)))}

#define GNEV_VERTEX_ATTR_INFO_LIST(r, data, elem) (BOOST_PP_SEQ_ELEM(0, elem))

#define GNEV_VERTEX_ATTR_OFFSET(name)                                                    \
    template <gnev::refl::Key K>                                                         \
    static consteval std::size_t GNEV_VERTEX_GET_ATTR_OFFSET_NAME() {                    \
        using Meta = gnev::refl::Meta<name>;                                             \
        return Meta::MemberOffset<K>();                                                  \
    }

#define GNEV_VERTEX(name, ...)                                                           \
    struct name {                                                                        \
        static constexpr std::size_t AttributesN = BOOST_PP_VARIADIC_SIZE(__VA_ARGS__);  \
        BOOST_PP_SEQ_FOR_EACH_I(GNEV_VERTEX_ATTR,                                        \
                                name,                                                    \
                                BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))                   \
        GNEV_VERTEX_GET_ATTR_INFO(name, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))           \
        GNEV_VERTEX_ATTR_OFFSET(name)                                                    \
    }

namespace details {

template <typename T>
concept hasStaticAttributesN = requires() {
    { T::AttributesN } -> std::convertible_to<std::size_t>;
};

template <typename T, std::size_t I>
concept hasStaticAttributeInfoI = hasStaticAttributesN<T>and
    requires()
{
    {
        T::template GNEV_VERTEX_GET_ATTR_INFO_NAME<I>()
    } -> std::convertible_to<VertexAttributeInfo>;
};

template <typename T>
concept hasStaticAttributeInfos = []<std::size_t... I>(std::index_sequence<I...>) {
    return (hasStaticAttributeInfoI<T, I> and ...);
}(std::make_index_sequence<T::AttributesN>{});

template <typename T, std::size_t I>
concept hasStaticAttributeOffsetI = hasStaticAttributesN<T>and
    requires()
{
    { T::template GNEV_VERTEX_GET_ATTR_OFFSET_NAME<I>() } -> std::same_as<std::size_t>;
};

template <typename T>
concept hasStaticAttributeOffsets = []<std::size_t... I>(std::index_sequence<I...>) {
    return (hasStaticAttributeOffsetI<T, I> and ...);
}(std::make_index_sequence<T::AttributesN>{});

} // namespace details

template <typename T>
concept IsVertex =
    details::hasStaticAttributesN<T> and details::hasStaticAttributeInfos<T> and
    details::hasStaticAttributeOffsets<T>;

} // namespace gnev::gl