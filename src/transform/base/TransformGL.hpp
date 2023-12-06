#pragma once

#include <concepts>
#include <type_traits>

namespace gnev::base {

template <typename T>
concept IsTransformGL = requires(T& t) {
    { T::InvalidIndex } -> std::convertible_to<unsigned int>;
    { t.parent } -> std::convertible_to<unsigned int>;
} and std::is_trivially_copyable_v<T> and std::is_standard_layout_v<T>;

} // namespace gnev::base