#pragma once

#include "data/VertexInfo.hpp"
#include "util/Util.hpp"

namespace gnev {
    
template<VertexAttributeInfo ... A>
struct EXPORT Vertex final {
public:
    static constexpr VertexInfo info {A...};
    static constexpr auto size = info.size;
    static constexpr auto count = info.count;

    template<size_t I, std::enable_if_t<(I >= 0 && I < info.count), bool> = true>
    using attribute_type = VertexAttribute<info.attributes[I]>;

    template<size_t I, std::enable_if_t<(I >= 0 && I < info.count), bool> = true>
    static constexpr auto attribute_size = info.attributes[I];

    template<size_t I, std::enable_if_t<(I >= 0 && I < info.count), bool> = true>
    static constexpr auto attribute_offset = info.offsets[I];

    Vertex(const VertexAttribute<A>&...);

    template<size_t I>
    auto& get();

    template<size_t I>
    const auto& get() const;

    GLbyte data[size];
    
private:
    template<size_t... Is>
    void _init_data(const auto& input, const std::index_sequence<Is...>&);

    template<size_t I>
    void _init_attrib_data(const auto& input);
};

template <typename derived>
struct is_Vertex
{
    template<VertexAttributeInfo... Ts>
    static constexpr std::true_type  test(const Vertex<Ts...> *);
    static constexpr std::false_type test(...);
    static constexpr bool value = decltype(test(std::declval<derived*>()))::value;
};

template <typename derived>
static constexpr bool is_Vertex_v = is_Vertex<derived>::value;

template <typename derived>
concept IsVertex = is_Vertex_v<derived>;

template<VertexAttributeInfo ... A>
Vertex<A...>::Vertex(const VertexAttribute<A>&... input)
{
    static_assert(sizeof(*this) == info.size);
    _init_data(std::forward_as_tuple(input...), std::make_index_sequence<sizeof...(A)>{});
}

template<VertexAttributeInfo ... A>
template<size_t I>
inline auto& Vertex<A...>::get()
{
    return *reinterpret_cast<attribute_type<I>*>(&data[attribute_offset<I>]);
}

template<VertexAttributeInfo ... A>
template<size_t I>
inline const auto& Vertex<A...>::get() const
{
    return *reinterpret_cast<const attribute_type<I>*>(&data[attribute_offset<I>]);
}

template<VertexAttributeInfo ... A>
template<size_t... Is>
void Vertex<A...>::_init_data(const auto& input, const std::index_sequence<Is...>&)
{
    (_init_attrib_data<Is>(input), ...);
}

template<VertexAttributeInfo ... A>
template<size_t I>
void Vertex<A...>::_init_attrib_data(const auto& input){
    get<I>() = std::get<I>(input);
}

}