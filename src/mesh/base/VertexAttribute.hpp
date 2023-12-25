#pragma once

#include "mesh/base/VertexAttributeInfo.hpp"

namespace gnev::base {

template <VertexAttributeInfo T>
struct VertexAttribute final {
    static constexpr VertexAttributeInfo info = T;
    using type = typename VertexAttributeType<T.type, info.elements>::type;
    using element_type =
        typename VertexAttributeType<T.type, info.elements>::element_type;

    VertexAttribute(const type& src)
        : data(src){};

    VertexAttribute(element_type v0)
        requires(info.elements == 1 && !info.is_packed)
        : data({v0}){};

    VertexAttribute(element_type v0, element_type v1)
        requires(info.elements == 2 && !info.is_packed)
        : data({v0, v1}){};

    VertexAttribute(element_type v0, element_type v1, element_type v2)
        requires(info.elements == 3 && !info.is_packed)
        : data({v0, v1, v2}){};

    VertexAttribute(element_type v0, element_type v1, element_type v2, element_type v3)
        requires(info.elements == 4 && !info.is_packed)
        : data({v0, v1, v2, v3}){};

    type data;
};

namespace details {

template <typename derived>
struct is_VertexAttribute {
    template <VertexAttributeInfo T>
    static constexpr std::true_type test(const VertexAttribute<T>*);
    static constexpr std::false_type test(...);
    static constexpr bool value = decltype(test(std::declval<derived*>()))::value;
};

} // namespace details

template <typename T>
concept IsVertexAttribute = details::is_VertexAttribute<T>::value;

} // namespace gnev::base