#pragma once

#include "data/VertexInfo.hpp"
#include "util/Util.hpp"

namespace gnev::data {

template <VertexAttributeInfo... A>
struct EXPORT Vertex final {
public:
    static constexpr VertexInfo info{A...};
    static constexpr auto size = info.size;
    static constexpr auto count = info.count;

    template <std::size_t I>
        requires(I >= 0 && I < count)
    using attribute_type = VertexAttribute<info.attributes[I]>;

    template <std::size_t I>
        requires(I >= 0 && I < count)
    static constexpr auto attribute_size = info.attributes[I];

    template <std::size_t I>
        requires(I >= 0 && I < count)
    static constexpr auto attribute_offset = info.offsets[I];

    Vertex(const VertexAttribute<A>&... input) {
        static_assert(sizeof(Vertex<A...>) == info.size);
        _init_data(std::forward_as_tuple(input...), std::make_index_sequence<sizeof...(A)>{});
    }

    template <std::size_t I>
        requires(I >= 0 && I < count)
    auto& get() {
        return *reinterpret_cast<attribute_type<I>*>(&data[attribute_offset<I>]);
    }

    template <std::size_t I>
        requires(I >= 0 && I < count)
    const auto& get() const {
        return *reinterpret_cast<const attribute_type<I>*>(&data[attribute_offset<I>]);
    }

    GLbyte data[size];

private:
    template <std::size_t... Is>
    void _init_data(const auto& input, const std::index_sequence<Is...>&) {
        (_init_attrib_data<Is>(input), ...);
    }

    template <std::size_t I>
    void _init_attrib_data(const auto& input) {
        get<I>() = std::get<I>(input);
    }
};

namespace details {

template <typename derived>
struct is_Vertex {
    template <VertexAttributeInfo... Ts>
    static constexpr std::true_type test(const Vertex<Ts...>*);
    static constexpr std::false_type test(...);
    static constexpr bool value = decltype(test(std::declval<derived*>()))::value;
};

} // namespace details

template <typename T>
concept IsVertex = details::is_Vertex<T>::value;

} // namespace gnev::data