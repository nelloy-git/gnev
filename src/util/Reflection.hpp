#pragma once

#include "pfr.hpp"
#include "util/CtString.hpp"

namespace gnev::refl {

namespace details {

template <typename T>
static constexpr bool HasDefaultAlignmentV =
    sizeof(T) == sizeof(decltype(pfr::structure_to_tuple<T>(std::declval<T>())));

template <typename T>
static constexpr bool IsReflectibleV =
    pfr::is_implicitly_reflectable_v<T, void> and HasDefaultAlignmentV<T>;

template <typename T>
concept IsReflectible = IsReflectibleV<T>;

}; // namespace details

template <details::IsReflectible T>
struct Meta {
    using Type = T;
    using Members = decltype(pfr::structure_to_tuple(std::declval<T>()));
    static constexpr auto Names = pfr::names_as_array<T>();

    template <CtString Key>
    struct KeyIndex {
        static consteval std::size_t getIndex() {
            std::size_t i = 0;
            for (i = 0; i < Names.size(); ++i) {
                if (Key == Names[i]) {
                    return i;
                }
            }
            throw std::out_of_range("");
        }

        static constexpr std::size_t value = getIndex();
    };

    struct Index {
        template <std::size_t I>
        static consteval Index Make() {
            return {I};
        }

        template <CtString Key>
        static consteval Index Make() {
            return {KeyIndex<Key>::value};
        }

        std::size_t value;
    };

    template <auto Key, auto... Next>
    struct DeduceMember;

    template <auto Key>
    struct DeduceMember<Key> {
        static constexpr std::size_t I = Index::template Make<Key>().value;
        using type = pfr::tuple_element_t<I, T>;
    };

    template <auto Key, auto... Next>
        requires(sizeof...(Next) > 0)
    struct DeduceMember<Key, Next...> {
        static constexpr std::size_t I = Index::template Make<Key>().value;
        static_assert(details::IsReflectibleV<pfr::tuple_element_t<I, T>>);
        using type =
            Meta<pfr::tuple_element_t<I, T>>::template DeduceMember<Next...>::type;
    };

    template <std::size_t I>
    using MemberI = pfr::tuple_element_t<I, T>;

    template <CtString Key>
    using MemberS = MemberI<KeyIndex<Key>::value>;

    template <auto Key>
    static consteval std::size_t Offset(std::size_t base = 0) {
        using Deduced = DeduceMember<Key>;
        constexpr std::size_t N = Deduced::I;

        auto get_local = []<std::size_t... I>(std::index_sequence<I...>) {
            return (sizeof(pfr::tuple_element_t<I, T>) + ... + 0);
        };
        std::size_t local = get_local(std::make_index_sequence<N>{});

        return base + local;
    }

    template <auto Key, auto... Next>
        requires(sizeof...(Next) > 0)
    static consteval std::size_t Offset(std::size_t base = 0) {
        using Deduced = DeduceMember<Key>;
        using NextMeta = Meta<Deduced::type>;
        constexpr std::size_t N = Deduced::I;

        auto get_local = []<std::size_t... I>(std::index_sequence<I...>) {
            return (sizeof(pfr::tuple_element_t<I, T>) + ... + 0);
        };
        std::size_t local = get_local(std::make_index_sequence<N>{});

        return NextMeta::template Offset<Next...>(base + local);
    }
};

} // namespace gnev::refl