#pragma once

#include "data/VertAttrib.hpp"

namespace gnev {

template<VertAttribT ... Attrs>
class Vertex {
public:
    using AttrTuple = std::tuple<Attrs...>;

    static constexpr GLuint Size = sizeof(AttrTuple);

    template<unsigned int I>
    using attrib_t = std::tuple_element_t<I, AttrTuple>;

    template<unsigned int I>
    auto& get_attrib();

    template<unsigned int I>
    static constexpr GLuint get_offset();

    template<unsigned int I>
    static constexpr GLuint get_size();

private:
    AttrTuple _attrib;

    template<std::size_t... I>
    static constexpr GLuint _get_offset(const std::index_sequence<I...>&);

};

template<VertAttribT ... Attrs>
template<unsigned int I>
auto& Vertex<Attrs...>::get_attrib()
{
    return std::get<I>(_attrib);
}

template<VertAttribT ... Attrs>
template<unsigned int I>
constexpr GLuint Vertex<Attrs...>::get_offset()
{
    if constexpr (I == 0){
        return 0;
    }
    return _get_offset(std::make_index_sequence<I>{});
}

template<VertAttribT ... Attrs>
template<unsigned int I>
constexpr GLuint Vertex<Attrs...>::get_size()
{
    return sizeof(std::tuple_element_t<I>);
}

template<VertAttribT ... Attrs>
template<std::size_t... I>
constexpr GLuint Vertex<Attrs...>::_get_offset(const std::index_sequence<I...>&)
{
    if constexpr (sizeof...(I) == 0){
        return 0;
    } else {
        return ((sizeof(std::tuple_element_t<I, decltype(_attrib)>)) + ...);
    }
}
        
template<template<VertAttribT ...> class base, typename derived>
struct is_Vertex_impl
{
    template<VertAttribT ... Args>
    static constexpr std::true_type  test(const base<Args...> *);
    static constexpr std::false_type test(...);
    static constexpr bool value = decltype(test(std::declval<derived*>()))::value;
};

template<typename T>
concept VertexT = is_Vertex_impl<Vertex, T>::value;

}