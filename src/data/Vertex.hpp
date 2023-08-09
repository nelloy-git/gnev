#pragma once

#include "data/Attrib.hpp"

namespace gnev {
    
template<AttribInfo ... A>
class Vertex {
    using helper_type = std::tuple<Attrib<A>...>;
public:
    template<size_t I>
    using attrib_type = std::tuple_element_t<I, helper_type>;

    Vertex(const Attrib<A>&...);

    static constexpr auto size = sizeof(helper_type);

    template<size_t I>
    auto& get_attrib();

    template<size_t I>
    const auto& get_attrib() const;

    template<size_t I>
    static constexpr GLuint get_size();

    template<size_t I>
    static constexpr GLuint get_offset();

    GLbyte data[sizeof(helper_type)];
    
private:
    template<size_t... I>
    static constexpr GLuint _get_offset(const std::index_sequence<I...>&);

    template<size_t... Is>
    void _init_data(const auto& input, const std::index_sequence<Is...>&);

    template<size_t I>
    void _init_attrib_data(const auto& input);
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
Vertex<A...>::Vertex(const Attrib<A>&... input)
{
    _init_data(std::forward_as_tuple(input...), std::make_index_sequence<sizeof...(A)>{});
}

template<AttribInfo ... A>
template<size_t I>
inline auto& Vertex<A...>::get_attrib()
{
    return *reinterpret_cast<attrib_type<I>*>(&data[get_offset<I>()]);
}

template<AttribInfo ... A>
template<size_t I>
inline const auto& Vertex<A...>::get_attrib() const
{
    return *reinterpret_cast<const attrib_type<I>*>(&data[get_offset<I>()]);
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

template<AttribInfo ... A>
template<size_t... Is>
void Vertex<A...>::_init_data(const auto& input, const std::index_sequence<Is...>&)
{
    (_init_attrib_data<Is>(input), ...);
}

template<AttribInfo ... A>
template<size_t I>
void Vertex<A...>::_init_attrib_data(const auto& input){
    get_attrib<I>() = std::get<I>(input);
}

}