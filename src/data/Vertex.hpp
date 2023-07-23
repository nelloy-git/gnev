#pragma once

#include "data/Attrib.hpp"

namespace gnev {

template<AttribInfo ... A>
class Vertex {
public:
    using type = std::tuple<Attrib<A>...>;
    template<size_t I>
    using attrib_type = std::tuple_element_t<I, type>;

    Vertex(const Attrib<A>&...);

    template<size_t I>
    auto& get_attrib();

    template<size_t I>
    static constexpr GLuint get_size();

    template<size_t I>
    static constexpr GLuint get_offset();

    type data;
    
private:
    template<size_t... I>
    static constexpr GLuint _get_offset(const std::index_sequence<I...>&);

};

template <typename derived>
struct is_Vertex
{
    template<AttribInfo... Ts>
    static constexpr std::true_type  test(const Vertex<Ts...> *);
    static constexpr std::false_type test(...);
    static constexpr bool value = decltype(test(std::declval<derived*>()))::value;
};

template <typename derived>
static constexpr bool is_Vertex_v = is_Vertex<derived>::value;

template <typename derived>
concept IsVertex = is_Vertex_v<derived>;

template<AttribInfo ... A>
Vertex<A...>::Vertex(const Attrib<A>&... data)
    : data({data...})
{

}

template<AttribInfo ... A>
template<size_t I>
inline auto& Vertex<A...>::get_attrib()
{
    return std::get<I>(data);
}

template<AttribInfo ... A>
template<size_t I>
constexpr GLuint Vertex<A...>::get_size()
{
    return sizeof(attrib_type<I>);
}

template<AttribInfo ... A>
template<size_t I>
constexpr GLuint Vertex<A...>::get_offset()
{
    if constexpr (I == 0){
        return 0;
    } else {
        return _get_offset(std::make_index_sequence<I>{});
    }
}

template<AttribInfo ... A>
template<size_t... I>
constexpr GLuint Vertex<A...>::_get_offset(const std::index_sequence<I...>&)
{
    if constexpr (sizeof...(I) == 0){
        return 0;
    } else {
        return (get_size<I>() + ...);
    }
}

}