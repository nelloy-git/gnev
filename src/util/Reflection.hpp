#pragma once

#include <array>
#include <cstddef>
#include <iostream>
#include <limits>
#include <utility>

#include "pfr/core.hpp"
#include "pfr/core_name.hpp"
#include "pfr/traits.hpp"
#include "pfr/tuple_size.hpp"
#include "quill/Fmt.h"
#include "quill/bundled/fmt/compile.h"
#include "quill/bundled/fmt/core.h"
#include "util/CtString.hpp"
#include "util/Logger.hpp"

#ifdef WIN32
#define GNEV_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define GNEV_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

namespace gnev {

template <typename T>
concept IsTriviallyCopyable = std::is_trivially_copyable_v<T>;

}

namespace gnev::refl {

namespace details {

template <typename T>
static constexpr bool HasDefaultAlignmentV =
    sizeof(T) == sizeof(decltype(pfr::structure_to_tuple<T>(std::declval<T>())));

}; // namespace details

template <typename T>
static constexpr bool IsReflectableV = []() {
    if constexpr (pfr::is_implicitly_reflectable_v<T, void>) {
        return details::HasDefaultAlignmentV<T>;
    }
    return false;
}();

template <typename T>
concept IsReflectable = IsReflectableV<T>;

template <IsReflectable T>
struct Meta;

template <std::size_t S = 1>
struct Key {
    consteval Key(const Key& other)
        : variant{other.variant}
        , value_index{other.value_index}
        , value_name{other.value_name} {}

    consteval Key(std::size_t index)
        : variant{Variant::Index}
        , value_index{index}
        , value_name{""} {}

    consteval Key(const CtString<S>& name)
        : variant{Variant::Name}
        , value_index{std::numeric_limits<std::size_t>::max()}
        , value_name{name} {}

    consteval Key(const char (&str)[S])
        : Key{CtString{str}} {}

    template <IsReflectable T>
    consteval std::size_t index() const {
        if (variant == Variant::Index) {
            return value_index;
        } else if (variant == Variant::Name) {
            constexpr auto Names = pfr::names_as_array<T>();
            std::size_t i = 0;
            for (i = 0; i < Names.size(); ++i) {
                if (value_name == Names[i]) {
                    return i;
                }
            }
            throw std::out_of_range("Key not found");
        } else {
            throw std::out_of_range("Unknown variant");
        }
    }

    template <IsReflectable T>
    consteval std::string_view name() const {
        if (variant == Variant::Index) {
            return pfr::names_as_array<T>()[value_index];
        } else if (variant == Variant::Name) {
            return value_name.to_string_view();
        } else {
            throw std::out_of_range("Unknown variant");
        }
    }

    enum class Variant {
        Index,
        Name
    };

    Variant variant;
    std::size_t value_index;
    CtString<S> value_name;
};

namespace details {

template <IsReflectable T, Key K>
static consteval std::size_t Offset(std::size_t base = 0) {
    auto get_local = []<std::size_t... I>(std::index_sequence<I...>) {
        return (sizeof(pfr::tuple_element_t<I, T>) + ... + 0);
    };
    std::size_t local = get_local(std::make_index_sequence<K.template index<T>()>{});

    return base + local;
}

} // namespace details

template <IsReflectable T, Key K>
struct MemberInfo {
    static constexpr std::size_t Index = K.template index<T>();
    static constexpr auto Name = pfr::get_name<Index, T>();
    static constexpr std::size_t Offset = details::Offset<T, K>();
    using Type = pfr::tuple_element_t<Index, T>;
};

template <IsReflectable T>
struct Meta {
    using Type = T;

    static constexpr std::size_t MembersN = pfr::tuple_size<T>();

    template <Key K, Key... Next>
    struct DeduceMemberInfo;

    template <Key K>
    struct DeduceMemberInfo<K> {
        using Type = MemberInfo<T, K>;
    };

    template <Key K, Key... Next>
    struct DeduceMemberInfo {
        using SubMeta = Meta<typename MemberInfo<T, K>::Type>;
        using Type = SubMeta::template DeduceMemberInfo<Next...>::Type;
    };

    template <Key K, Key... Next>
    using MemberInfo = DeduceMemberInfo<K, Next...>::Type;

    template <Key K, Key... Next>
    using MemberType = MemberInfo<K, Next...>::Type;

    template <Key K, Key... Next>
    using MemberMeta = Meta<MemberType<K, Next...>>;

    template <Key K, Key... Next>
    using MemberName = MemberInfo<K, Next...>::Name;

    template <Key K>
    static consteval std::size_t MemberOffset(std::size_t base = 0) {
        return base + MemberInfo<K>::Offset;
    };

    template <Key K, Key... Next>
        requires(sizeof...(Next) > 0)
    static consteval std::size_t MemberOffset(std::size_t base = 0) {
        return MemberMeta<K>::template MemberOffset<Next...>(base + MemberOffset<K>());
    };
};

template <typename T, std::size_t Align>
struct Aligned {
    static consteval std::size_t GetReserveSize() {
        std::size_t n = sizeof(T) / Align;
        return n * Align == sizeof(T) ? 0 : (n + 1) * Align - sizeof(T);
    }

    template <std::size_t Size>
    struct Reserve {
        std::array<std::byte, Size> _;
    };

    template <>
    struct Reserve<0> {};

    T value;
    GNEV_NO_UNIQUE_ADDRESS Reserve<GetReserveSize()> reserve;
};

} // namespace gnev::refl