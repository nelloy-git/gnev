#pragma once

#include <limits>
#include <type_traits>

#include "material/base/Define.hpp"
#include "util/Util.hpp"

namespace gnev::base {

template <MaterialTexIndex TexS>
struct EXPORT alignas(16) MaterialGL {
public:
    static constexpr MaterialTexIndex TexSize = TexS;

    MaterialGL(MaterialTexIndex initial_tex_index =
                   std::numeric_limits<MaterialTexIndex>::max()) {
        std::fill_n(tex_index.begin(), TexS, initial_tex_index);
    }

    std::array<MaterialTexIndex, TexS> tex_index;
};

namespace details {

template <typename derived>
struct is_MaterialGL {
    template <MaterialTexIndex TexS>
    static constexpr std::true_type test(const MaterialGL<TexS>*);
    static constexpr std::false_type test(...);
    static constexpr bool value = decltype(test(std::declval<derived*>()))::value;
};

} // namespace details

template <typename T>
concept IsMaterialGL =
    details::is_MaterialGL<T>::value and std::is_trivially_copyable_v<T>;

} // namespace gnev::base