#pragma once

#include <type_traits>

#include "util/CtString.hpp"

namespace gnev::refl {

namespace details {

template <typename... Metas>
consteval std::array<std::size_t, sizeof...(Metas)> initIndexSize() {
    return {Metas::Size...};
}

template <typename... Metas>
consteval std::array<std::size_t, sizeof...(Metas)>
initIndexOffset(const std::array<std::size_t, sizeof...(Metas)>& IndexSize) {
    std::array<std::size_t, sizeof...(Metas)> arr;
    arr[0] = 0;
    for (std::size_t i = 1; i < sizeof...(Metas); ++i) {
        arr[i] = arr[i - 1] + IndexSize[i - 1];
    }
    return arr;
}

template <CtString Key, typename... Pairs, std::size_t... I>
consteval std::size_t findKeyIndex(std::index_sequence<I...>) {
    using TuplePairs = std::tuple<Pairs...>;
    constexpr std::size_t PairsN = sizeof...(Pairs);

    std::array<std::size_t, PairsN> eqKeys = {
        (Key == std::tuple_element_t<I, TuplePairs>::Key ? I : PairsN)...};

    std::size_t found = PairsN;
    for (std::size_t i = 0; i < eqKeys.size(); ++i) {
        if (eqKeys[i] != PairsN) {
            if (found != PairsN) {
                throw std::out_of_range("Found at least 2 same keys");
            }
            found = eqKeys[i];
        }
    }
    if (found == PairsN) {
        throw std::out_of_range("Key not found");
    }

    return found;
}

} // namespace details

template <typename T, std::size_t S = sizeof(T)>
struct FieldLeafMeta final {
    using Type = T;
    static constexpr std::size_t Size = S;
};

template <typename... Metas_>
struct FieldArrayMeta final {
    static_assert((IsFieldMetaV<Metas_> && ...));

    using SubMetas = std::tuple<Metas_...>;
    template <std::size_t I>
    using SubMeta = std::tuple_element_t<I, SubMetas>;

    static constexpr std::size_t N = sizeof...(Metas_);
    static constexpr std::size_t Size = (Metas_::Size + ...);

    static constexpr std::array<std::size_t, N> IndexSize =
        details::initIndexSize<Metas_...>();
    static constexpr std::array<std::size_t, N> IndexOffset =
        details::initIndexOffset<Metas_...>(IndexSize);

    template <std::size_t I>
    static consteval std::size_t Offset(std::size_t base = 0) {
        return base + IndexOffset[I];
    }

    template <std::size_t I, auto... Next>
        requires(sizeof...(Next) > 0)
    static consteval std::size_t Offset(std::size_t base = 0) {
        return SubMeta<I>::template Offset<Next...>(base + IndexOffset[I]);
    }
};

template <CtString K, typename V>
struct FieldMapMetaPair final {
    static_assert(IsFieldMetaV<V>);

    static constexpr CtString Key = K;
    using Value = V;
};

template <typename... Pairs_>
struct FieldMapMeta final {
    static_assert((IsFieldMapMetaPairV<Pairs_> && ...));

    template <CtString Key>
    static constexpr std::size_t KeyIndex =
        details::findKeyIndex<Key,
                              Pairs_...>(std::make_index_sequence<sizeof...(Pairs_)>{});

    using Pairs = std::tuple<Pairs_...>;
    using SubMetas = std::tuple<typename Pairs_::Value...>;
    template <CtString Key>
    using SubMeta = std::tuple_element_t<KeyIndex<Key>, SubMetas>;

    static constexpr std::size_t N = sizeof...(Pairs_);
    static constexpr std::size_t Size = (Pairs_::Value::Size + ...);

    static constexpr std::array<std::size_t, N> IndexSize =
        details::initIndexSize<typename Pairs_::Value...>();
    static constexpr std::array<std::size_t, N> IndexOffset =
        details::initIndexOffset<typename Pairs_::Value...>(IndexSize);

    template <CtString Key>
    static consteval std::size_t Offset(std::size_t base = 0) {
        return base + IndexOffset[KeyIndex<Key>];
    }

    template <CtString Key, auto... Next>
        requires(sizeof...(Next) > 0)
    static consteval std::size_t Offset(std::size_t base = 0) {
        return SubMeta<Key>::template Offset<Next...>(base + IndexOffset[KeyIndex<Key>]);
    }
};

namespace details {

template <typename Pair>
struct IsFieldMapMetaPairImpl {
    template <CtString K, typename V>
    static constexpr std::true_type test(const FieldMapMetaPair<K, V>*);
    static constexpr std::false_type test(...);
    using type = decltype(test(std::declval<Pair*>()));
};

template <typename Leaf>
struct IsFieldLeafMetaImpl {
    template <typename T, std::size_t S>
    static constexpr std::true_type test(const FieldLeafMeta<T, S>*);
    static constexpr std::false_type test(...);
    using type = decltype(test(std::declval<Leaf*>()));
};

template <typename Array>
struct IsFieldArrayMetaImpl {
    template <typename... Metas>
    static constexpr std::true_type test(const FieldArrayMeta<Metas...>*);
    static constexpr std::false_type test(...);
    using type = decltype(test(std::declval<Array*>()));
};

template <typename Map>
struct IsFieldMapMetaImpl {
    template <typename... Pairs>
    static constexpr std::true_type test(const FieldMapMeta<Pairs...>*);
    static constexpr std::false_type test(...);
    using type = decltype(test(std::declval<Map*>()));
};

} // namespace details

template <typename T>
static constexpr bool IsFieldMapMetaPairV =
    details::IsFieldMapMetaPairImpl<T>::type::value;

template <typename T>
static constexpr bool IsFieldLeafMetaV = details::IsFieldLeafMetaImpl<T>::type::value;

template <typename T>
static constexpr bool IsFieldArrayMetaV = details::IsFieldArrayMetaImpl<T>::type::value;

template <typename T>
static constexpr bool IsFieldMapMetaV = details::IsFieldMapMetaImpl<T>::type::value;

template <typename T>
static constexpr bool IsFieldMetaV =
    IsFieldLeafMetaV<T> || IsFieldArrayMetaV<T> || IsFieldMapMetaV<T>;

template <typename T>
concept IsFieldLeafMeta = IsFieldLeafMetaV<T>;

template <typename T>
concept IsFieldArrayMeta = IsFieldArrayMetaV<T>;

template <typename T>
concept IsFieldMapMeta = IsFieldMapMetaV<T>;

template <typename T>
concept IsFieldMeta = IsFieldMetaV<T>;

} // namespace gnev::refl