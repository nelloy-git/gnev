#pragma once

#include <concepts>
#include <type_traits>

#include "glm/mat4x4.hpp"

namespace gnev::base {

template <typename T>
concept IsTransformGL = requires(T& t) {
    { T::InvalidIndex } -> std::convertible_to<unsigned int>;
    { t.parent } -> std::convertible_to<unsigned int>;
    { t.mat } -> std::convertible_to<glm::mat4>;
} and std::is_trivially_copyable_v<T> and std::is_standard_layout_v<T>;

} // namespace gnev