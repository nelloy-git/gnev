#pragma once

#include <concepts>
#include <limits>
#include <type_traits>

#include "util/Util.hpp"

namespace gnev::base {

template <typename T>
concept IsMaterialGL = requires(T& t) {
    { T::TexSize } -> std::convertible_to<unsigned int>;
    { T::InvalidTexIndex } -> std::convertible_to<unsigned int>;
    { t.tex_index } -> std::convertible_to<std::array<unsigned int, T::TexSize>&>;
} and std::is_trivially_copyable_v<T> and std::is_standard_layout_v<T>;

} // namespace gnev::base