#pragma once

#include <limits>
#include <type_traits>

#include "material/base/Define.hpp"
#include "util/Util.hpp"

namespace gnev::base {

template <typename T>
concept IsMaterialGL = requires(T& t) {
    { t.tex_index } -> std::convertible_to<std::array<MaterialTexIndex, T::TexSize>&>;
} and std::is_trivially_copyable_v<T> and std::is_standard_layout_v<T>;

} // namespace gnev::base