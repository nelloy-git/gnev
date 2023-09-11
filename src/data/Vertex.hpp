#pragma once

#include "data/VertexInfo.hpp"
#include "util/Util.hpp"

namespace gnev {

template <VertexAttributeInfo... A> struct EXPORT Vertex final {
public:
  static constexpr VertexInfo info{A...};
  static constexpr auto size = info.size;
  static constexpr auto count = info.count;

  template <size_t I, std::enable_if_t<(I >= 0 && I < info.count), bool> = true>
  using attribute_type = VertexAttribute<info.attributes[I]>;

  template <size_t I, std::enable_if_t<(I >= 0 && I < info.count), bool> = true>
  static constexpr auto attribute_size = info.attributes[I];

  template <size_t I, std::enable_if_t<(I >= 0 && I < info.count), bool> = true>
  static constexpr auto attribute_offset = info.offsets[I];

  Vertex(const VertexAttribute<A> &...input) {
    static_assert(sizeof(Vertex<A...>) == info.size);
    _init_data(std::forward_as_tuple(input...),
               std::make_index_sequence<sizeof...(A)>{});
  }

  template <size_t I> auto &get() {
    return *reinterpret_cast<attribute_type<I> *>(&data[attribute_offset<I>]);
  }

  template <size_t I> const auto &get() const {
    return *reinterpret_cast<const attribute_type<I> *>(
        &data[attribute_offset<I>]);
  }

  GLbyte data[size];

private:
  template <size_t... Is>
  void _init_data(const auto &input, const std::index_sequence<Is...> &) {
    (_init_attrib_data<Is>(input), ...);
  }

  template <size_t I> void _init_attrib_data(const auto &input) {
    get<I>() = std::get<I>(input);
  }
};

template <typename derived> struct is_Vertex {
  template <VertexAttributeInfo... Ts>
  static constexpr std::true_type test(const Vertex<Ts...> *);
  static constexpr std::false_type test(...);
  static constexpr bool value =
      decltype(test(std::declval<derived *>()))::value;
};

template <typename derived>
static constexpr bool is_Vertex_v = is_Vertex<derived>::value;

template <typename derived>
concept IsVertex = is_Vertex_v<derived>;

template <typename T>
concept IsIndex = std::is_same_v<T, GLubyte> || std::is_same_v<T, GLushort> ||
                  std::is_same_v<T, GLuint>;

template <IsIndex T>
constexpr GLenum IndexEnum = std::is_same_v<T, GLubyte>    ? GL_UNSIGNED_BYTE
                             : std::is_same_v<T, GLushort> ? GL_UNSIGNED_SHORT
                             : std::is_same_v<T, GLuint>   ? GL_UNSIGNED_INT
                                                           : 0;
;

} // namespace gnev